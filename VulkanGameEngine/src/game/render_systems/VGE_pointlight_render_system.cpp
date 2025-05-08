#include "pch.h"
#include "VGE_pointlight_render_system.hpp"
#include "component.hpp"


namespace VGE
{

    struct SimplePushConstantData
    {
        glm::mat4 modelMatrix{1.0f};
        glm::mat4 normalMatrix{1.0f};
    };

    VgePointLightRenderSystem::VgePointLightRenderSystem(VgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : _device{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }
    VgePointLightRenderSystem::~VgePointLightRenderSystem()
    {
        vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
    }

    void VgePointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.offset = 0;
        // pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void VgePointLightRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(_pipelineLayout != VK_NULL_HANDLE && "Pipeline cannot be created before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VgePipeline::defaultPipelineConfigInfo(pipelineConfig);

        // don't use the default values (which is tied to the Vertex structure)
        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.bindingDescription.clear();

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = _pipelineLayout;
        _pipeline = std::make_unique<VgePipeline>(_device, "shaders/point_light.vert.spv", "shaders/point_light.frag.spv", pipelineConfig);
    }

    void VgePointLightRenderSystem::render(FrameInfo& frameInfo)
    {
        _pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        // TODO: Remove hard coded values below
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}