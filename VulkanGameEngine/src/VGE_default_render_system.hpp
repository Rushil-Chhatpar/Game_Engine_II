#pragma once

#include "VGE_device.hpp"
#include "VGE_pipeline.hpp"
#include "VGE_mesh.hpp"
#include "VGE_game_object.hpp"

#include <memory>
#include <vector>

namespace VGE
{
    class VgeDefaultRenderSystem
    {
    public:

        VgeDefaultRenderSystem(VgeDevice& device, VkRenderPass renderPass);
        ~VgeDefaultRenderSystem();

        VgeDefaultRenderSystem(const VgeDefaultRenderSystem &) = delete;
        VgeDefaultRenderSystem& operator=(const VgeDefaultRenderSystem &) = delete;


        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VgeGameObject>& gameObjects);
    private:
    
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

    private:
        VgeDevice& _device;

        std::unique_ptr<VgePipeline> _pipeline;
        VkPipelineLayout _pipelineLayout;

        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
    };
}