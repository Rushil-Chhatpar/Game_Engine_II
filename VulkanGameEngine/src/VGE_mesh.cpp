#include "VGE_mesh.hpp"

#include <cassert>
#include <cstring>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/tinyobjloader/tiny_obj_loader.h"

namespace VGE
{
    VgeMesh::VgeMesh(VgeDevice& device, const std::vector<Vertex>& vertices)
        : _device(device)
    {
        createVertexBuffer(vertices);
    }

    VgeMesh::VgeMesh(VgeDevice& device, const char* filename)
        : _device(device)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;
    
        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filename))
        {
            throw std::runtime_error("Failed to load obj!!!   " + warning + error);
        }
    }

    VgeMesh::~VgeMesh()
    {
        vkDestroyBuffer(_device.device(), _vertexBuffer, nullptr);
        vkFreeMemory(_device.device(), _vertexBufferMemory, nullptr);
    }
    
    void VgeMesh::createVertexBuffer(const std::vector<Vertex> &vertices)
    {
        _vertexCount = static_cast<uint32_t>(vertices.size());
        assert(_vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
        
        _device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vertexBuffer, _vertexBufferMemory);
        
        void* data;
        vkMapMemory(_device.device(), _vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(_device.device(), _vertexBufferMemory);
    }

    // void VgeMesh::sierpinskiTriangle(std::vector<Vertex> &vertices, uint32_t depth, const glm::vec2 &left, const glm::vec2 &right, const glm::vec2 &top)
    // {
    //     glm::vec3 red = {1.0f, 0.0f, 0.0f};
    //     glm::vec3 green = {0.0f, 1.0f, 0.0f};
    //     glm::vec3 blue = {0.0f, 0.0f, 1.0f};
    //     if (depth <= 0)
    //     {
    //         vertices.push_back({{top}, {green}});
    //         vertices.push_back({{right}, {green}});
    //         vertices.push_back({{left}, {green}});
    //     } 
    //     else 
    //     {
    //         auto leftTop = 0.5f * (left + top);
    //         auto rightTop = 0.5f * (right + top);
    //         auto leftRight = 0.5f * (left + right);
    //         sierpinskiTriangle(vertices, depth - 1, left, leftRight, leftTop);
    //         sierpinskiTriangle(vertices, depth - 1, leftRight, right, rightTop);
    //         sierpinskiTriangle(vertices, depth - 1, leftTop, rightTop, top);
    //     }
    // }

    void VgeMesh::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
    }

    void VgeMesh::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { _vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> VgeMesh::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> VgeMesh::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
}