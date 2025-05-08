#pragma once

#include "VGE_device.hpp"
#include "VGE_pipeline.hpp"
#include "game_object.hpp"
#include "VGE_frame_info.hpp"

#include <memory>
#include <vector>

namespace VGE
{

    class VgePointLightRenderSystem
    {
    public:

        VgePointLightRenderSystem(VgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout setLayouts);
        ~VgePointLightRenderSystem();

        VgePointLightRenderSystem(const VgePointLightRenderSystem &) = delete;
        VgePointLightRenderSystem& operator=(const VgePointLightRenderSystem &) = delete;

        // We don't make the camera a member variable as I want to able to share camera between multiple render systems
        void render(FrameInfo& frameInfo);

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