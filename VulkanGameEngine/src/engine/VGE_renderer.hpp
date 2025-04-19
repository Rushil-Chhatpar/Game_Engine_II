#pragma once

#include "VGE_device.hpp"
#include "VGE_window.hpp"
#include "VGE_swap_chain.hpp"
#include "VGE_mesh.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace VGE
{
    class VgeRenderer
    {
    public:
        VgeRenderer(VgeWindow& window, VgeDevice& device);
        ~VgeRenderer();

        VgeRenderer(const VgeRenderer &) = delete;
        VgeRenderer& operator=(const VgeRenderer &) = delete;

        bool isFrameStarted() const { return _isFrameStarted; }

        VkRenderPass getSwapChainRenderPass() const { return _swapChain->getRenderPass(); }
        VkCommandBuffer getCurrentCommandBuffer() const 
        {
            assert(_isFrameStarted && "Cannot get command buffer when frame is not in progress");
            return _commandBuffers[_currentFrameIndex];
        }
        float getAspectRatio() const { return _swapChain->extentAspectRatio(); }

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        int getFrameIndex() const
        {
            assert(_isFrameStarted && "Cannot get frame index when frame is not in progress");
            return _currentFrameIndex;
        }

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

    private:
        VgeWindow& _window;
        VgeDevice& _device;
        std::unique_ptr<VgeSwapChain> _swapChain;
        std::vector<VkCommandBuffer> _commandBuffers;

        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};

        uint32_t _currentImgIndex;
        int _currentFrameIndex;
        bool _isFrameStarted;
    };
}