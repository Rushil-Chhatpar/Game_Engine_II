#include "pch.h"
#include "VGE_descriptors.hpp"


namespace VGE
{
    VgeDescriptorSetLayout::Builder &VgeDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
    {
        assert(_bindings.count(binding) == 0 && "Binding already in use");
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = count;
        layoutBinding.stageFlags = stageFlags;
        _bindings[binding] = layoutBinding;
        return *this;
    }

    std::unique_ptr<VgeDescriptorSetLayout> VgeDescriptorSetLayout::Builder::build()
    {
        return std::make_unique<VgeDescriptorSetLayout>(_device, _bindings);
    }

    VgeDescriptorSetLayout::VgeDescriptorSetLayout(VgeDevice &device, const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> &bindings)
        : _device(device)
        , _bindings(bindings)
    {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
        for (auto binding : _bindings)
        {
            setLayoutBindings.push_back(binding.second);
        }

        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        createInfo.pBindings = setLayoutBindings.data();

        if(vkCreateDescriptorSetLayout(_device.device(), &createInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create descriptor set layout");
        }
    }

    VgeDescriptorSetLayout::~VgeDescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(_device.device(), _descriptorSetLayout, nullptr);
    }

    VgeDescriptorPool::Builder &VgeDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
    {
        _poolSizes.push_back({descriptorType, count});
        return *this;
    }

    VgeDescriptorPool::Builder &VgeDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
    {
        _poolFlags = flags;
        return *this;
    }

    VgeDescriptorPool::Builder &VgeDescriptorPool::Builder::setMaxSets(uint32_t count)
    {
        _maxSets = count;
        return *this;
    }

    std::unique_ptr<VgeDescriptorPool> VgeDescriptorPool::Builder::build()
    {
        return std::make_unique<VgeDescriptorPool>(_device, _maxSets, _poolFlags, _poolSizes);
    }

    VgeDescriptorPool::VgeDescriptorPool(VgeDevice &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes)
        : _device(device)
    {
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;
        poolInfo.flags = poolFlags;
        if (vkCreateDescriptorPool(_device.device(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create descriptor pool");
        }
    }

    VgeDescriptorPool::~VgeDescriptorPool()
    {
        vkDestroyDescriptorPool(_device.device(), _descriptorPool, nullptr);
    }

    bool VgeDescriptorPool::allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = _descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;

        if (vkAllocateDescriptorSets(_device.device(), &allocInfo, &descriptor) != VK_SUCCESS)
        {
            return false;
        }
        return true;
    }

    void VgeDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const
    {
        vkFreeDescriptorSets(_device.device(), _descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
    }

    void VgeDescriptorPool::resetPool()
    {
        vkResetDescriptorPool(_device.device(), _descriptorPool, 0);
    }

    VgeDescriptorWriter::VgeDescriptorWriter(VgeDescriptorSetLayout &setLayout, VgeDescriptorPool &pool)
        : _setLayout(setLayout)
        , _pool(pool)
    {
    }

    VgeDescriptorWriter &VgeDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo)
    {
        assert(_setLayout._bindings.count(binding) == 1 && "Cannot write to descriptor set layout binding");

        auto &bindingDescription = _setLayout._bindings[binding];

        assert(bindingDescription.descriptorCount == 1 && "Cannot write to descriptor set layout binding with count > 1");
        VkWriteDescriptorSet writeDescriptorSet{};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstBinding = binding;
        writeDescriptorSet.pBufferInfo = bufferInfo;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = bindingDescription.descriptorType;

        _writes.push_back(writeDescriptorSet);
        return *this;
    }

    VgeDescriptorWriter &VgeDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo)
    {
        assert(_setLayout._bindings.count(binding) == 1 && "Cannot write to descriptor set layout binding");

        auto &bindingDescription = _setLayout._bindings[binding];

        assert(bindingDescription.descriptorCount == 1 && "Cannot write to descriptor set layout binding with count > 1");
        VkWriteDescriptorSet writeDescriptorSet{};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstBinding = binding;
        writeDescriptorSet.pImageInfo = imageInfo;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = bindingDescription.descriptorType;

        _writes.push_back(writeDescriptorSet);
        return *this;
    }

    bool VgeDescriptorWriter::build(VkDescriptorSet &set)
    {
        bool result = _pool.allocateDescriptorSet(_setLayout.getDescriptorSetLayout(), set);
        if (!result)
        {
            return false;
        }
        overwrite(set);
        return true;
    }

    void VgeDescriptorWriter::overwrite(VkDescriptorSet &set)
    {
        for (auto &write : _writes)
        {
            write.dstSet = set;
        }
        vkUpdateDescriptorSets(_pool._device.device(), static_cast<uint32_t>(_writes.size()), _writes.data(), 0, nullptr);
    }
}