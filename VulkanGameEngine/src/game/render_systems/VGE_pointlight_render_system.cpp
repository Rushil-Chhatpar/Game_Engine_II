#include "pch.h"
#include "VGE_pointlight_render_system.hpp"
#include "component.hpp"


namespace VGE
{

    struct PointLightPushConstant
    {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius{};
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
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstant);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

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

    void VgePointLightRenderSystem::update(FrameInfo &frameInfo, GlobalUBO &ubo, std::vector<game::GameObject*>& gameObjects)
    {
        int lightIndex = 0;
        for(game::GameObject*& gameObject: gameObjects)
        {
            ubo.pointLights[lightIndex].position = glm::vec4(gameObject->Transform->translation, 1.0f);
            ubo.pointLights[lightIndex].color = glm::vec4(gameObject->getComponent<game::PointLightComponent>()->getColor(), gameObject->getComponent<game::PointLightComponent>()->getIntensity());
            lightIndex++;
        }
        ubo.numPointLights = static_cast<int>(gameObjects.size());
    }

    void VgePointLightRenderSystem::render(FrameInfo &frameInfo, std::vector<game::GameObject*>& gameObjects)
    {
        // TODO: Find way to batch render the lights
        _pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for(game::GameObject*& gameObject: gameObjects)
        {
            PointLightPushConstant push{};
            push.position = glm::vec4(gameObject->Transform->translation, 1.0f);
            push.color = glm::vec4(gameObject->getComponent<game::PointLightComponent>()->getColor(), gameObject->getComponent<game::PointLightComponent>()->getIntensity());
            push.radius = gameObject->getComponent<game::PointLightComponent>()->getRadius();

            vkCmdPushConstants(frameInfo.commandBuffer, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstant), &push);
            
            vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
        }

    }
}