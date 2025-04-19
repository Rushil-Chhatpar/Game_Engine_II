#pragma once

#include "VGE_device.hpp"

namespace VGE
{
    class VgeBuffer {
        public:
         VgeBuffer(
             VgeDevice& device,
             VkDeviceSize instanceSize,
             uint32_t instanceCount,
             VkBufferUsageFlags usageFlags,
             VkMemoryPropertyFlags memoryPropertyFlags,
             VkDeviceSize minOffsetAlignment = 1);
         ~VgeBuffer();
        
         VgeBuffer(const VgeBuffer&) = delete;
         VgeBuffer& operator=(const VgeBuffer&) = delete;
        
         VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
         void unmap();
        
         void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
         VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
         VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
         VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        
         void writeToIndex(void* data, int index);
         VkResult flushIndex(int index);
         VkDescriptorBufferInfo descriptorInfoForIndex(int index);
         VkResult invalidateIndex(int index);
        
         VkBuffer getBuffer() const { return _buffer; }
         void* getMappedMemory() const { return _mapped; }
         uint32_t getInstanceCount() const { return _instanceCount; }
         VkDeviceSize getInstanceSize() const { return _instanceSize; }
         VkDeviceSize getAlignmentSize() const { return _alignmentSize; }
         VkBufferUsageFlags getUsageFlags() const { return _usageFlags; }
         VkMemoryPropertyFlags getMemoryPropertyFlags() const { return _memoryPropertyFlags; }
         VkDeviceSize getBufferSize() const { return _bufferSize; }
        
        private:
         static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);
        
         VgeDevice& _device;
         void* _mapped = nullptr;
         VkBuffer _buffer = VK_NULL_HANDLE;
         VkDeviceMemory _memory = VK_NULL_HANDLE;
        
         VkDeviceSize _bufferSize;
         uint32_t _instanceCount;
         VkDeviceSize _instanceSize;
         VkDeviceSize _alignmentSize;
         VkBufferUsageFlags _usageFlags;
         VkMemoryPropertyFlags _memoryPropertyFlags;
    };
}