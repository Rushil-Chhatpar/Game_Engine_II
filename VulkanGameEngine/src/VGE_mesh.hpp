#pragma once

#include "VGE_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <memory>

// #define MAKE_HASHABLE(type, ...) \
//     namespace std {\
//         template<> struct hash<type> {\
//             std::size_t operator()(const type &t) const {\
//                 std::size_t ret = 0;\
//                 hash_combine(ret, __VA_ARGS__);\
//                 return ret;\
//             }\
//         };\
//     }

namespace VGE
{
    class VgeMesh
    {
    public:

        struct Vertex
        {
            glm::vec3 pos{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const
            {
                return pos == other.pos && color == other.color && 
                       normal == other.normal && uv == other.uv;
            }
        };

        struct MeshBuilder
        {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            MeshBuilder& addVertex(const Vertex& vertex)
            {
                vertices.push_back(vertex);
                return *this;
            }

            MeshBuilder& addIndex(uint32_t index)
            {
                indices.push_back(index);
                return *this;
            }

            void loadWavefront(const char* filename);
        };

        VgeMesh(VgeDevice& device, const MeshBuilder& builder);
        VgeMesh(VgeDevice& device, const char* filename);
        ~VgeMesh();

        VgeMesh(const VgeMesh&) = delete;
        VgeMesh& operator=(const VgeMesh&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        static std::unique_ptr<VgeMesh> createModelFromFile(VgeDevice& device, const char* filename);        

        //static void sierpinskiTriangle(std::vector<Vertex>& vertices, uint32_t depth, const glm::vec2& left, const glm::vec2& right, const glm::vec2& top);
    
    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);
        void createIndexBuffer(const std::vector<uint32_t>& indices);
        // void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        // void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        // void createCommandBuffer();
        // void createCommandPool();

    private:
        VgeDevice& _device;

        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        uint32_t _vertexCount;

        bool _hasIndexBuffer = false;
        VkBuffer _indexBuffer;
        VkDeviceMemory _indexBufferMemory;
        uint32_t _indexCount;
    };
}

namespace std {
    template<> struct hash<VGE::VgeMesh::Vertex> {
        size_t operator()(const VGE::VgeMesh::Vertex& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1)
                   ^ (hash<glm::vec3>()(vertex.normal) << 1)
                   ^ (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}