#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_vulkan.h"

namespace game
{
    struct GuiInitInfo
    {
        uint32_t apiVersion = VK_API_VERSION_1_0;
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
        uint32_t queueFamily;
        VkQueue queue;
        VkDescriptorPool descriptorPool;
        VkRenderPass renderPass;
        uint32_t minImageCount;
        uint32_t imageCount;
        VkSampleCountFlagBits MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    };

    class GuiManager
    {
        DECLARE_SINGLETON(GuiManager);
    public:
        void Init(GLFWwindow* window, GuiInitInfo& initInfo);
        void Cleanup();

        void BeginFrame();
        void EndFrame(VkCommandBuffer commandBuffer);

        void RenderMainFrame();
        void RenderInspectorFrame();

    private:
        const char* _dockspaceName = "MyDockSpace";
    };
}