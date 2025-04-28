#include "pch.h"
#include "gui_manager.hpp"

namespace game
{
    DEFINE_SINGLETON(GuiManager);

    void GuiManager::Init(GLFWwindow* window, GuiInitInfo& initInfo)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(window, true);

        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = initInfo.instance;
        init_info.PhysicalDevice = initInfo.physicalDevice;
        init_info.Device = initInfo.logicalDevice;
        init_info.QueueFamily = initInfo.queueFamily;
        init_info.Queue = initInfo.queue;
        init_info.DescriptorPool = initInfo.descriptorPool;
        init_info.RenderPass = initInfo.renderPass;
        init_info.MinImageCount = initInfo.minImageCount;
        init_info.ImageCount = initInfo.imageCount;
        init_info.MSAASamples = initInfo.MSAASamples;

        ImGui_ImplVulkan_Init(&init_info);
    }

    void GuiManager::Cleanup()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiManager::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GuiManager::EndFrame(VkCommandBuffer commandBuffer)
    {
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
    }

    void GuiManager::RenderMainFrame()
    {
        // Setup a main window with no frame and a dockspace that covers the entire viewport.
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
    
        if (ImGui::Begin("Main Dock", nullptr, flags))
        {
            ImGuiID dockspaceID = ImGui::GetID("My Dockspace");
            ImGui::DockSpace(dockspaceID);
        }
        ImGui::End();
    }
}