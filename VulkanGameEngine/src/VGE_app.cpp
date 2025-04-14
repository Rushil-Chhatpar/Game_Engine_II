#include "VGE_app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>
#include <array>

namespace VGE
{

    struct SimplePushConstantData
    {
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    VgeApp::VgeApp()
    {
        loadMeshes();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    VgeApp::~VgeApp()
    {
        vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
    }

    void VgeApp::run()
    {
        while (!_window.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(_device.device());
    }

    void VgeApp::loadMeshes()
    {
        std::vector<VgeMesh::Vertex> vertices{};
        VgeMesh::sierpinskiTriangle(vertices, 6, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        
        _mesh = std::make_unique<VgeMesh>(_device, vertices);
    }

    void VgeApp::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void VgeApp::createPipeline()
    {
        assert(_swapChain != nullptr && "Swap chain is null. Cannot create pipeline");
        assert(_pipelineLayout != VK_NULL_HANDLE && "Pipeline cannot be created before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VgePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = _swapChain->getRenderPass();
        pipelineConfig.pipelineLayout = _pipelineLayout;
        _pipeline = std::make_unique<VgePipeline>(_device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void VgeApp::createCommandBuffers()
    {
        // For now we will use the same command buffer for all frames
        // In the future we will use a command buffer per frame
        _commandBuffers.resize(_swapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _device.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

        if (vkAllocateCommandBuffers(_device.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void VgeApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(_device.device(), _device.getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
        _commandBuffers.clear();
    }

    void VgeApp::drawFrame()
    {
        uint32_t imageIndex;
        // Acquire the next image from the swap chain
        // This will block until an image is available
        // The image index is used to identify which image we will be rendering to
        auto result = _swapChain->acquireNextImage(&imageIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }
        
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);

        // Submit the command buffer to the queue
        // This will block until the command buffer has finished executing
        result = _swapChain->submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized())
        {
            _window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if(result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present the swap chain image!");
        }
    }

    void VgeApp::recreateSwapChain()
    {
        VkExtent2D extent = _window.getExtent();
        // wait until the window is resized
        // This is a blocking call that will wait until the window is resized
        while (extent.width == 0 || extent.height == 0)
        {
            extent = _window.getExtent();
            glfwWaitEvents();
        }

        // wait for the device to finish executing all commands
        vkDeviceWaitIdle(_device.device());

        if(_swapChain == nullptr)
        {
            _swapChain = std::make_unique<VgeSwapChain>(_device, extent);
        }
        else
        {
            // If the swap chain already exists, we need to recreate it
            // This will move the old swap chain and create a new one
            // with the new extent
            // We need to pass the old swap chain to the new swap chain
            // so that it can be reused
            _swapChain = std::make_unique<VgeSwapChain>(_device, extent, std::move(_swapChain));
            if(_swapChain->imageCount() != _commandBuffers.size())
            {
                // reset command buffers
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        _swapChain = std::make_unique<VgeSwapChain>(_device, extent);
        createPipeline();

    }

    void VgeApp::recordCommandBuffer(uint32_t imageIndex)
    {
        static int frame = 0;
        frame = (frame + 1) % 1000;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        // The first command is to begin a render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _swapChain->getRenderPass();
        renderPassInfo.framebuffer = _swapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _swapChain->getSwapChainExtent();

        // This corresponds to what we want the initial values of the frame buffer attachments to be cleared to
        // In this case we want to clear the color attachment to green and the depth attachment to 1.0
        // In our render pass, index 0 is the color attachment and index 1 is the depth attachment
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = _defaultClearColor;
        clearValues[1].depthStencil = {1.0f, 0};

        // Provide clear values to our render pass
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        // VK_SUBPASS_CONTENTS_INLINE means we will record commands directly into the command buffer and no secondary command buffers will be used
        vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(_swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(_swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = _swapChain->getSwapChainExtent();

        vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);

        _pipeline->bind(_commandBuffers[imageIndex]);

        // bind and draw the meshes
        _mesh->bind(_commandBuffers[imageIndex]);
        
        for(int i = 0;i<4;i++)
        {
            SimplePushConstantData push{};
            push.offset = {-0.5f + frame * 0.002f, -0.4f + i * 0.25f};
            push.color = {0.0f, 0.0f, 0.2f + 0.2f * i};

            vkCmdPushConstants(_commandBuffers[imageIndex], _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            _mesh->draw(_commandBuffers[imageIndex]);
        }

        vkCmdEndRenderPass(_commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}