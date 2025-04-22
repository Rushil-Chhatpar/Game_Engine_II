#pragma once

#include "VGE_camera.hpp"
#include "VGE_device.hpp"
#include "VGE_pipeline.hpp"
#include "game_object.hpp"
#include "VGE_frame_info.hpp"

#include <memory>
#include <vector>

namespace VGE
{
    class VgeDefaultRenderSystem
    {
    public:

        VgeDefaultRenderSystem(VgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~VgeDefaultRenderSystem();

        VgeDefaultRenderSystem(const VgeDefaultRenderSystem &) = delete;
        VgeDefaultRenderSystem& operator=(const VgeDefaultRenderSystem &) = delete;

        // We don't make the camera a member variable as I want to able to share camera between multiple render systems
        void renderGameObjects(FrameInfo& frameInfo, std::vector<game::GameObject*>& gameObjects);
    private:
    
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

    private:
        VgeDevice& _device;

        std::unique_ptr<VgePipeline> _pipeline;
        VkPipelineLayout _pipelineLayout;

        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
    };
}