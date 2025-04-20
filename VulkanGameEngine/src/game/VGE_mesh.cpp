#include "pch.h"
#include "VGE_mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

namespace VGE
{
    VgeMesh::VgeMesh(VgeDevice& device, const MeshBuilder& builder)
        : _device(device)
    {
        createVertexBuffer(builder.vertices);
        createIndexBuffer(builder.indices);
    }

    VgeMesh::~VgeMesh()
    {
    }
    
    void VgeMesh::createVertexBuffer(const std::vector<Vertex> &vertices)
    {
        _vertexCount = static_cast<uint32_t>(vertices.size());
        assert(_vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        VgeBuffer stagingBuffer(_device, vertexSize, _vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)vertices.data());

        _vertexBuffer = std::make_unique<VgeBuffer>(_device, vertexSize, _vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        _device.copyBuffer(stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), bufferSize);
    }

    void VgeMesh::createIndexBuffer(const std::vector<uint32_t> &indices)
    {
        _indexCount = static_cast<uint32_t>(indices.size());
        _hasIndexBuffer = _indexCount > 0;

        if(!_hasIndexBuffer)
        {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indices[0]) * _indexCount;
        uint32_t indexSize = sizeof(indices[0]);

        VgeBuffer stagingBuffer(_device, indexSize, _indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)indices.data());
        
        _indexBuffer = std::make_unique<VgeBuffer>(_device, indexSize, _indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        
        _device.copyBuffer(stagingBuffer.getBuffer(), _indexBuffer->getBuffer(), bufferSize);
    }

    void VgeMesh::draw(VkCommandBuffer commandBuffer)
    {
        if(_hasIndexBuffer)
        {
            vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
        }
    }

    std::unique_ptr<VgeMesh> VgeMesh::createModelFromFile(VgeDevice &device, const char *filename)
    {
        MeshBuilder builder;
        builder.loadWavefront(filename);

        return std::make_unique<VgeMesh>(device, builder);
    }

    void VgeMesh::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { _vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        if(_hasIndexBuffer)
        {
            vkCmdBindIndexBuffer(commandBuffer, _indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
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
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, uv);

        return attributeDescriptions;
    }

    void VgeMesh::MeshBuilder::loadWavefront(const char *filename)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filename))
        {
            throw std::runtime_error("Failed to load obj!!!   " + warning + error);
        }
        
        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (tinyobj::shape_t& shape : shapes)
	    {
		    for (auto index : shape.mesh.indices)
		    {
			    Vertex vertex{};
                if(index.vertex_index >=0)
                {
			        vertex.pos = {
			    	    attrib.vertices[3 * index.vertex_index + 0],
			    	    attrib.vertices[3 * index.vertex_index + 1],
			    	    attrib.vertices[3 * index.vertex_index + 2],
			        };
                   
                    vertex.color = {
			    	    attrib.colors[3 * index.vertex_index + 0],
			    	    attrib.colors[3 * index.vertex_index + 1],
			    	    attrib.colors[3 * index.vertex_index + 2],
			        };
                }

                if(index.texcoord_index >=0)
                {
			        vertex.uv = {
			    	    attrib.texcoords[2 * index.texcoord_index + 0],
			    	    1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
			        };
                }

                if(index.normal_index >=0)
                {
                    vertex.normal = {
			    	    attrib.normals[3 * index.normal_index + 0],
			    	    attrib.normals[3 * index.normal_index + 1],
			    	    attrib.normals[3 * index.normal_index + 2],
			        };
                }

			    if(uniqueVertices.count(vertex) == 0)
			    {
			    	uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
			    	vertices.push_back(vertex);
			    }
			    indices.push_back(uniqueVertices[vertex]);
		    }
	    }
    }
}