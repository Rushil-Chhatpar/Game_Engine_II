#pragma once

#include "VGE_device.hpp"

#include <memory>
#include <unordered_map>
#include <vector>


namespace VGE
{

    class VgeDescriptorSetLayout
    {
    public:
        class Builder
        {
        public:
            Builder(VgeDevice& device) : _device{device} {}

            Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);

            std::unique_ptr<VgeDescriptorSetLayout> build();
        private:
            VgeDevice& _device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> _bindings{};
        };

        VgeDescriptorSetLayout(VgeDevice& device, const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings);
        ~VgeDescriptorSetLayout();

        VgeDescriptorSetLayout(const VgeDescriptorSetLayout&) = delete;
        VgeDescriptorSetLayout& operator=(const VgeDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return _descriptorSetLayout; }

    private:
        VgeDevice& _device;
        VkDescriptorSetLayout _descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> _bindings{};

        friend class VgeDescriptorWriter;
    };

    class VgeDescriptorPool
    {
    public:
        class Builder
        {
        public:
            Builder(VgeDevice& device) : _device{device} {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);

            std::unique_ptr<VgeDescriptorPool> build();

        private:
            VgeDevice& _device;
            std::vector<VkDescriptorPoolSize> _poolSizes{};
            uint32_t _maxSets = 1000;
            VkDescriptorPoolCreateFlags _poolFlags = 0;
        };

        VgeDescriptorPool(VgeDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~VgeDescriptorPool();
        VgeDescriptorPool(const VgeDescriptorPool&) = delete;
        VgeDescriptorPool& operator=(const VgeDescriptorPool&) = delete;

        // Allocate a single descriptor set from the pool
        bool allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
        void resetPool();

    private:
        VgeDevice& _device;
        VkDescriptorPool _descriptorPool;

        friend class VgeDescriptorWriter;
    };

    class VgeDescriptorWriter
    {
    public:
        VgeDescriptorWriter(VgeDescriptorSetLayout& setLayout, VgeDescriptorPool& pool);

        VgeDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VgeDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);
        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

        private:
        VgeDescriptorSetLayout& _setLayout;
        VgeDescriptorPool& _pool;
        std::vector<VkWriteDescriptorSet> _writes{};
    };


}