#include "pch.h"
#include "gui_manager.hpp"
#include "scene_manager.hpp"

namespace game
{
    DEFINE_SINGLETON(GuiManager);

    void GuiManager::Init(GLFWwindow* window, GuiInitInfo& initInfo)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
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
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = false;
        bool opt_fullscreen = opt_fullscreen_persistant;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if(opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);
        if(opt_fullscreen)
        {
            ImGui::PopStyleVar(2);
        }
        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspaceID = ImGui::GetID(_dockspaceName);
            ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
        ImGui::End();
    }

    void GuiManager::RenderInspectorFrame()
    {
        ImGui::Begin("Inspector");
        GET_SINGLETON(SceneManager)->GUI_DisplayObjectList();
        ImGui::End();

        ImGui::Begin("Properties");
        GET_SINGLETON(SceneManager)->GUI_DisplayObjectProperties();
        ImGui::End();
    }
}