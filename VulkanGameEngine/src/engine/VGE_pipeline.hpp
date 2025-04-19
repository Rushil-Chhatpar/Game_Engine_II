#pragma once

#include "VGE_device.hpp"
#include <string>
#include <vector>

namespace VGE
{
    struct PipelineConfigInfo
    {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportStateInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationStateInfo;
        VkPipelineMultisampleStateCreateInfo multisampleStateInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendStateInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class VgePipeline
    {
    public:
        VgePipeline(VgeDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
        ~VgePipeline();

        VgePipeline(const VgePipeline&) = delete;
        VgePipeline& operator=(const VgePipeline&) = delete;

        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        void bind(VkCommandBuffer commandBuffer);

    private:
        static std::vector<char> readFile(const std::string& filePath);
        
        void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    private:
        VgeDevice& _device;
        VkPipeline _graphicsPipeline;
        VkShaderModule _vertShaderModule;
        VkShaderModule _fragShaderModule;
    };
}