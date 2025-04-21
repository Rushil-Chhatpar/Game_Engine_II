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

    void VgeDefaultRenderSystem::renderGameObjects(FrameInfo& frameInfo, std::vector<game::GameObject>& gameObjects)
    {
        _pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for(auto& gameObject : gameObjects)
        {
            SimplePushConstantData push{};
            push.modelMatrix = gameObject.Transform->mat4();
            push.normalMatrix = gameObject.Transform->normalMatrix();

            vkCmdPushConstants(frameInfo.commandBuffer, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            gameObject.getMesh()->bind(frameInfo.commandBuffer);
            gameObject.getMesh()->draw(frameInfo.commandBuffer);
        }
    }
}