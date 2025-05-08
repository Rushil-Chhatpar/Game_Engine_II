#include "pch.h"
#include  "VGE_default_render_system.hpp"
#include "component.hpp"


namespace VGE
{

    struct SimplePushConstantData
    {
        glm::mat4 modelMatrix{1.0f};
        glm::mat4 normalMatrix{1.0f};
    };

    VgeDefaultRenderSystem::VgeDefaultRenderSystem(VgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : _device{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }
    VgeDefaultRenderSystem::~VgeDefaultRenderSystem()
    {
        vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
    }

    void VgeDefaultRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void VgeDefaultRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(_pipelineLayout != VK_NULL_HANDLE && "Pipeline cannot be created before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VgePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = _pipelineLayout;
        _pipeline = std::make_unique<VgePipeline>(_device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void VgeDefaultRenderSystem::createBatchBuffers(const std::vector<game::GameObject *> &gameObjects)
    {
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        std::vector<VkDeviceSize> vertexOffsets;
        std::vector<VkDeviceSize> indexOffsets;
        std::vector<uint32_t> indexCounts;

        VkDeviceSize vertexBufferSize = 0;
        VkDeviceSize indexBufferSize = 0;

        for (const auto& gameObject : gameObjects)
        {
            VgeMesh* mesh = gameObject->getComponent<game::MeshComponent>()->getMesh();

            vertexOffsets.push_back(vertexBufferSize);
            indexOffsets.push_back(indexBufferSize);
            indexCounts.push_back(mesh->getIndexCount());
            vertexBufferSize += mesh->getVertexCount() * sizeof(VgeMesh::Vertex);
            indexBufferSize += mesh->getIndexCount() * sizeof(uint32_t);
            totalVertexCount += mesh->getVertexCount();
            totalIndexCount += mesh->getIndexCount();
        }

        // Create the vertex and index buffers
        _batchData.vertexBuffer = std::make_unique<VgeBuffer>(_device, vertexBufferSize, 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        _batchData.indexBuffer = std::make_unique<VgeBuffer>(_device, indexBufferSize, 1, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // create staging buffers
        std::unique_ptr<VgeBuffer> stagingVertexBuffer = std::make_unique<VgeBuffer>(_device, vertexBufferSize, 1, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        std::unique_ptr<VgeBuffer> stagingIndexBuffer = std::make_unique<VgeBuffer>(_device, indexBufferSize, 1, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingVertexBuffer->map(vertexBufferSize, 0);
        VkDeviceSize vertexOffset = 0;
        for(const auto& gameObject : gameObjects)
        {
            VgeMesh* mesh = gameObject->getComponent<game::MeshComponent>()->getMesh();
            void* srcData = mesh->getVertexBuffer()->getMappedMemory();
            memcpy(static_cast<char*>(stagingVertexBuffer->getMappedMemory()) + vertexOffset, srcData,
                     mesh->getVertexCount() * sizeof(VgeMesh::Vertex));
            vertexOffset += mesh->getVertexCount() * sizeof(VgeMesh::Vertex);
        }
        stagingVertexBuffer->unmap();

        stagingIndexBuffer->map(indexBufferSize, 0);
        VkDeviceSize indexOffset = 0;
        uint32_t vertexBase = 0;
        for(const auto& gameObject : gameObjects)
        {
            VgeMesh* mesh = gameObject->getComponent<game::MeshComponent>()->getMesh();
            void* srcData = mesh->getIndexBuffer()->getMappedMemory();
            const uint32_t* indices = static_cast<const uint32_t*>(srcData);
            uint32_t* dstIndices = static_cast<uint32_t*>(stagingIndexBuffer->getMappedMemory()) + indexOffset;
            for (uint32_t i = 0; i < mesh->getIndexCount(); ++i)
            {
                dstIndices[i] = indices[i] + vertexBase;
            }
            indexOffset += mesh->getIndexCount() * sizeof(uint32_t);
            vertexBase += mesh->getVertexCount();
        }
        stagingIndexBuffer->unmap();

        // copy to device local buffer
        _device.copyBuffer(stagingVertexBuffer->getBuffer(), _batchData.vertexBuffer->getBuffer(), vertexBufferSize);
        _device.copyBuffer(stagingIndexBuffer->getBuffer(), _batchData.indexBuffer->getBuffer(), indexBufferSize);

        // create instance buffer
        VkDeviceSize instanceBufferSize = sizeof(InstanceData) * gameObjects.size();
        _instanceBuffer = std::make_unique<VgeBuffer>(_device, instanceBufferSize, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        
        // set batch data
        _batchData.totalVertexCount = totalVertexCount;
        _batchData.totalIndexCount = totalIndexCount;
        _batchData.vertexOffsets = vertexOffsets;
        _batchData.indexOffsets = indexOffsets;
        _batchData.indexCounts = indexCounts;
    }

    void VgeDefaultRenderSystem::renderGameObjects(FrameInfo& frameInfo, std::vector<game::GameObject*>& gameObjects)
    {
        _pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for(auto& gameObject : gameObjects)
        {
            SimplePushConstantData push{};
            push.modelMatrix = gameObject->Transform->mat4();
            push.normalMatrix = gameObject->Transform->normalMatrix();

            vkCmdPushConstants(frameInfo.commandBuffer, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            gameObject->getComponent<game::MeshComponent>()->getMesh()->bind(frameInfo.commandBuffer);
            gameObject->getComponent<game::MeshComponent>()->getMesh()->draw(frameInfo.commandBuffer);
        }
    }

    void VgeDefaultRenderSystem::renderGameObjectsinBatch(FrameInfo &frameInfo, std::vector<game::GameObject*>& gameObjects)
    {
        _pipeline->bind(frameInfo.commandBuffer);

        std::vector<InstanceData> instanceData(gameObjects.size());
        for(size_t i = 0; i < gameObjects.size(); i++)
        {
            instanceData[i].modelMatrix = gameObjects[i]->Transform->mat4();
            instanceData[i].normalMatrix = gameObjects[i]->Transform->normalMatrix();
        }

        _instanceBuffer->writeToBuffer(instanceData.data(), sizeof(InstanceData) * instanceData.size(), 0);


        VkBuffer vertexBuffers[] = {_batchData.vertexBuffer->getBuffer()};
        VkDeviceSize vertexOffsets[] = {0};
        vkCmdBindVertexBuffers(frameInfo.commandBuffer, 0, 1, vertexBuffers, vertexOffsets);
        vkCmdBindIndexBuffer(frameInfo.commandBuffer, _batchData.indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        uint32_t firstIndex = 0;
        uint32_t vertexIndexOffset = 0;
        for(uint32_t i = 0;i<_batchData.indexCounts.size();i++)
        {
            vkCmdDrawIndexed(frameInfo.commandBuffer, _batchData.indexCounts[i], 1, firstIndex, vertexIndexOffset, 0);
            firstIndex += _batchData.indexCounts[i];
            vertexIndexOffset += (_batchData.vertexOffsets[i + 1] - _batchData.vertexOffsets[i]) / sizeof(VgeMesh::Vertex);
        }
    }
}