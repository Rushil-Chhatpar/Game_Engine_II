#pragma once

#include "VGE_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace VGE
{
    class VgeMesh
    {
    public:

        struct Vertex
        {
            glm::vec2 pos;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        VgeMesh(VgeDevice& device, const std::vector<Vertex>& vertices);
        ~VgeMesh();

        VgeMesh(const VgeMesh&) = delete;
        VgeMesh& operator=(const VgeMesh&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        static void sierpinskiTriangle(std::vector<Vertex>& vertices, uint32_t depth, const glm::vec2& left, const glm::vec2& right, const glm::vec2& top);
    
    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);
        // void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        // void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        // void createCommandBuffer();
        // void createCommandPool();

    private:
        VgeDevice& _device;
        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        uint32_t _vertexCount;
    };
}