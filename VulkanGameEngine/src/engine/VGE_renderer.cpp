#include "VGE_renderer.hpp"
#include <stdexcept>
#include <array>


namespace VGE
{

    VgeRenderer::VgeRenderer(VgeWindow& window, VgeDevice& device)
        : _window{window}, _device{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    VgeRenderer::~VgeRenderer()
    {
        freeCommandBuffers();
    }

    VkCommandBuffer VgeRenderer::beginFrame()
    {
        assert(!_isFrameStarted && "Frame is already in progress");

        // Acquire the next image from the swap chain
        // This will block until an image is available
        // The image index is used to identify which image we will be rendering to
        auto result = _swapChain->acquireNextImage(&_currentImgIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }
        
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        _isFrameStarted = true;

        VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        return commandBuffer;
    }

    void VgeRenderer::endFrame()
    {
        assert(_isFrameStarted && "Cannot end frame because frame is not in progress");
        VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }

        VkResult result = _swapChain->submitCommandBuffers(&commandBuffer, &_currentImgIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized())
        {
            _window.resetWindowResizedFlag();
            recreateSwapChain();
        }
        if(result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present the swap chain image!");
        }

        _isFrameStarted = false;
        _currentFrameIndex = (_currentFrameIndex + 1) % VgeSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void VgeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(_isFrameStarted && "Cannot begin render pass because frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Command buffer is not the current command buffer");

        // The first command is to begin a render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _swapChain->getRenderPass();
        renderPassInfo.framebuffer = _swapChain->getFrameBuffer(_currentImgIndex);

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
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void VgeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(_isFrameStarted && "Cannot end render pass because frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Command buffer is not the current command buffer");

        vkCmdEndRenderPass(commandBuffer);
    }

    void VgeRenderer::createCommandBuffers()
    {
        // For now we will use the same command buffer for all frames
        // In the future we will use a command buffer per frame
        _commandBuffers.resize(VgeSwapChain::MAX_FRAMES_IN_FLIGHT);

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

    void VgeRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(_device.device(), _device.getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
        _commandBuffers.clear();
    }

    void VgeRenderer::recreateSwapChain()
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
            std::shared_ptr<VgeSwapChain> oldSwapChain = std::move(_swapChain);
            // If the swap chain already exists, we need to recreate it
            // This will move the old swap chain and create a new one
            // with the new extent
            // We need to pass the old swap chain to the new swap chain
            // so that it can be reused
            _swapChain = std::make_unique<VgeSwapChain>(_device, extent, oldSwapChain);

            if(!oldSwapChain->compareSwapFormats(*_swapChain.get()))
            {
                throw std::runtime_error("Swap chain image or depth format has changed!");
            }
        }
        _swapChain = std::make_unique<VgeSwapChain>(_device, extent);
    }
}