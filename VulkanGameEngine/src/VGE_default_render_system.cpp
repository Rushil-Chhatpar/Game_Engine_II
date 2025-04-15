#include  "VGE_default_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <iostream>
#include <array>

namespace VGE
{

    struct SimplePushConstantData
    {
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    VgeDefaultRenderSystem::VgeDefaultRenderSystem(VgeDevice& device, VkRenderPass renderPass)
        : _device{device}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }
    VgeDefaultRenderSystem::~VgeDefaultRenderSystem()
    {
        vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
    }

    void VgeDefaultRenderSystem::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void VgeDefaultRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(_pipelineLayout != VK_NULL_HANDLE && "Pipeline cannot be created before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VgePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = _pipelineLayout;
        _pipeline = std::make_unique<VgePipeline>(_device, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void VgeDefaultRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VgeGameObject>& gameObjects)
    {
        int i = 0;
        for (auto& gameObject : gameObjects)
        {
            i++;
            gameObject.Transform2d.rotation = glm::mod<float>(gameObject.Transform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
        }

        _pipeline->bind(commandBuffer);

        for(auto& gameObject : gameObjects)
        {
            gameObject.Transform2d.rotation = glm::mod(gameObject.Transform2d.rotation + 0.01f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = gameObject.Transform2d.translation;
            push.color = gameObject.getColor();
            push.transform = gameObject.Transform2d.mat2();

            vkCmdPushConstants(commandBuffer, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            gameObject.getMesh()->bind(commandBuffer);
            gameObject.getMesh()->draw(commandBuffer);
        }
    }
}