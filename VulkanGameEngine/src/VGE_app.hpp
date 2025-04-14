#pragma once

#include "VGE_device.hpp"
#include "VGE_pipeline.hpp"
#include "VGE_window.hpp"
#include "VGE_swap_chain.hpp"
#include "VGE_mesh.hpp"

#include <memory>
#include <vector>

namespace VGE
{
    class VgeApp
    {
    public:
        static constexpr int WIDTH = 1920;
        static constexpr int HEIGHT = 1080;

        VgeApp();
        ~VgeApp();

        VgeApp(const VgeApp &) = delete;
        VgeApp& operator=(const VgeApp &) = delete;

        void run();

    private:
    
        void loadMeshes();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(uint32_t imageIndex);

    private:
        VgeWindow _window{WIDTH, HEIGHT, "Vulkan Game Engine"};
        VgeDevice _device{_window};
        std::unique_ptr<VgeSwapChain> _swapChain;
        std::unique_ptr<VgePipeline> _pipeline;
        VkPipelineLayout _pipelineLayout;
        std::vector<VkCommandBuffer> _commandBuffers;
        std::unique_ptr<VgeMesh> _mesh;

        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
    };
}