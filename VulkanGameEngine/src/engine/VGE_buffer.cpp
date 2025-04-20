/*
 * Encapsulates a vulkan buffer
 *
 * Based off VulkanBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h
 */


 #include "pch.h"
 #include "VGE_buffer.hpp"


namespace VGE
{
    VgeBuffer::VgeBuffer(VgeDevice &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
        : _device(device)
        , _instanceSize(instanceSize)
        , _instanceCount(instanceCount)
        , _usageFlags(usageFlags)
        , _memoryPropertyFlags(memoryPropertyFlags)
    {
        _alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
        _bufferSize = instanceCount * _alignmentSize;
        _device.createBuffer(
            _bufferSize,
            usageFlags,
            memoryPropertyFlags,
            _buffer,
            _memory);
    }

    VgeBuffer::~VgeBuffer()
    {
        unmap();
        if (_buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(_device.device(), _buffer, nullptr);
        }
        if (_memory != VK_NULL_HANDLE)
        {
            vkFreeMemory(_device.device(), _memory, nullptr);
        }
    }

    VkResult VgeBuffer::map(VkDeviceSize size, VkDeviceSize offset)
    {
        return vkMapMemory(_device.device(), _memory, offset, size, 0, &_mapped);
    }
    
    void VgeBuffer::unmap()
    {
        if(_mapped)
        {
            vkUnmapMemory(_device.device(), _memory);
            _mapped = nullptr;
        }
    }
    
    void VgeBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
    {
        assert(_mapped && "Cannot copy to buffer: no mapped memory. Did you call map?");

        if(size == VK_WHOLE_SIZE)
        {
            memcpy(_mapped, data, _bufferSize);
        }
        else
        {
            char* memoryOffset = (char*)_mapped;
            memoryOffset += offset;
            memcpy(memoryOffset, data, size);
        }
    }

    VkResult VgeBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
    {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = _memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(_device.device(), 1, &mappedRange);
    }

    VkResult VgeBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
    {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = _memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(_device.device(), 1, &mappedRange);
    }

    VkDescriptorBufferInfo VgeBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
    {
        return VkDescriptorBufferInfo{_buffer, offset, size};
    }

    void VgeBuffer::writeToIndex(void *data, int index)
    {
        writeToBuffer(data, _instanceSize, index * _alignmentSize);
    }

    VkResult VgeBuffer::flushIndex(int index)
    {
        return flush(_alignmentSize, index * _alignmentSize);
    }

    VkDescriptorBufferInfo VgeBuffer::descriptorInfoForIndex(int index)
    {
        return descriptorInfo(_alignmentSize, index * _alignmentSize);
    }

    VkResult VgeBuffer::invalidateIndex(int index)
    {
        return invalidate(_alignmentSize, index * _alignmentSize);
    }

    VkDeviceSize VgeBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
    {
        if(minOffsetAlignment > 0)
        {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        else
        {
            return instanceSize;
        }
    }
}