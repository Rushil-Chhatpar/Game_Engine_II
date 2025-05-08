#pragma once

#include "VGE_device.hpp"
#include "VGE_pipeline.hpp"
#include "game_object.hpp"
#include "VGE_frame_info.hpp"

#include <memory>
#include <vector>

namespace VGE
{

    struct InstanceData
    {
        glm::mat4 modelMatrix;
        glm::mat3 normalMatrix;
        // float textureIndex = 0; // Not used for now
    };

    struct BatchData
    {
        std::unique_ptr<VgeBuffer> vertexBuffer;
        std::unique_ptr<VgeBuffer> indexBuffer;
        std::vector<uint32_t> indexCounts;
        std::vector<VkDeviceSize> vertexOffsets;
        std::vector<VkDeviceSize> indexOffsets; 
        uint32_t totalVertexCount;
        uint32_t totalIndexCount;
    };

    class VgeDefaultRenderSystem
    {
    public:

        VgeDefaultRenderSystem(VgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout setLayouts);
        ~VgeDefaultRenderSystem();

        VgeDefaultRenderSystem(const VgeDefaultRenderSystem &) = delete;
        VgeDefaultRenderSystem& operator=(const VgeDefaultRenderSystem &) = delete;

        // We don't make the camera a member variable as I want to able to share camera between multiple render systems
        void renderGameObjects(FrameInfo& frameInfo, std::vector<game::GameObject*>& gameObjects);

        void renderGameObjectsinBatch(FrameInfo& frameInfo, std::vector<game::GameObject*>& gameObjects);
        void createBatchBuffers(const std::vector<game::GameObject*>& gameObjects);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

    private:
        VgeDevice& _device;

        std::unique_ptr<VgePipeline> _pipeline;
        VkPipelineLayout _pipelineLayout;

        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};

        BatchData _batchData;
        std::unique_ptr<VgeBuffer> _instanceBuffer;
    };
}