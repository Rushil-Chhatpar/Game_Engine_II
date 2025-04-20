#include "pch.h"
#include "VGE_pipeline.hpp"
#include "VGE_mesh.hpp"

#include <fstream>


namespace VGE
{
    VgePipeline::VgePipeline(VgeDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo)
    : _device{device}
    {
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    VgePipeline::~VgePipeline()
    {
        vkDestroyShaderModule(_device.device(), _fragShaderModule, nullptr);
        vkDestroyShaderModule(_device.device(), _vertShaderModule, nullptr);
        vkDestroyPipeline(_device.device(), _graphicsPipeline, nullptr);
    }

    std::vector<char> VgePipeline::readFile(const std::string &filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    void VgePipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo)
    {
        assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
        assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in configInfo");

        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        createShaderModule(vertCode, &_vertShaderModule);
        createShaderModule(fragCode, &_fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = _vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = _fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].pSpecializationInfo = nullptr;
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;

        auto bindingDescriptions = VgeMesh::Vertex::getBindingDescriptions();
        auto attributeDescriptions = VgeMesh::Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &configInfo.viewportStateInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationStateInfo;
        pipelineInfo.pMultisampleState = &configInfo.multisampleStateInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilStateInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendStateInfo;
        pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;
        
        pipelineInfo.layout = configInfo.pipelineLayout;
        pipelineInfo.renderPass = configInfo.renderPass;
        pipelineInfo.subpass = configInfo.subpass;

        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        if(vkCreateGraphicsPipelines(_device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }

    void VgePipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
        if (vkCreateShaderModule(_device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    void VgePipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
    {
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        configInfo.inputAssemblyInfo.pNext = nullptr;

        configInfo.viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportStateInfo.viewportCount = 1;
        configInfo.viewportStateInfo.pViewports = nullptr;
        configInfo.viewportStateInfo.scissorCount = 1;
        configInfo.viewportStateInfo.pScissors = nullptr;

        configInfo.rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationStateInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationStateInfo.lineWidth = 1.0f;
        configInfo.rasterizationStateInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationStateInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationStateInfo.depthBiasConstantFactor = 0.0f;
        configInfo.rasterizationStateInfo.depthBiasClamp = 0.0f;
        configInfo.rasterizationStateInfo.depthBiasSlopeFactor = 0.0f;

        configInfo.multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleStateInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleStateInfo.minSampleShading = 1.0f;
        configInfo.multisampleStateInfo.pSampleMask = nullptr;
        configInfo.multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
        configInfo.multisampleStateInfo.alphaToOneEnable = VK_FALSE;

        configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        configInfo.colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendStateInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
        configInfo.colorBlendStateInfo.attachmentCount = 1;
        configInfo.colorBlendStateInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendStateInfo.blendConstants[0] = 0.0f;
        configInfo.colorBlendStateInfo.blendConstants[1] = 0.0f;
        configInfo.colorBlendStateInfo.blendConstants[2] = 0.0f;
        configInfo.colorBlendStateInfo.blendConstants[3] = 0.0f;
        configInfo.colorBlendStateInfo.pNext = nullptr;

        configInfo.depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilStateInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilStateInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilStateInfo.minDepthBounds = 0.0f;
        configInfo.depthStencilStateInfo.maxDepthBounds = 1.0f;
        configInfo.depthStencilStateInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilStateInfo.front = {};
        configInfo.depthStencilStateInfo.back = {};

        configInfo.dynamicStateEnables = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };
        configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
        configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
        configInfo.dynamicStateInfo.pNext = nullptr;
        configInfo.dynamicStateInfo.flags = 0;
    }

    void VgePipeline::bind(VkCommandBuffer commandBuffer)
    {
        // The VK_PIPELINE_BIND_POINT_GRAPHICS enum is used to specify that we are binding a graphics pipeline
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
    }
}