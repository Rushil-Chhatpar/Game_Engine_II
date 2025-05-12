#include "pch.h"
#include "game_object.hpp"
#include "scene.hpp"
#include "VGE_app.hpp"
#include "gui_manager.hpp"

namespace game
{
    GameObject::GameObject(id_t id, std::string name, Scene* scene)
        : _id(id), _name(name), _scene(scene)
    {
        _guiSelectables.push_back(GuiSelectable("Point Light",
            std::bind(&GameObject::AddPointLightComponent, this)));
        _guiSelectables.push_back(GuiSelectable("Keyboard Controller",
            std::bind(&GameObject::AddKBControllerComponent, this)));
        _guiSelectables.push_back(GuiSelectable("Mesh", nullptr,
            std::bind(&GameObject::GUI_DisplayAddMeshComponentMenu, this)));
    }

    void GameObject::AddKBControllerComponent()
    {
        this->addComponent<KeyboardController>(_scene->getApp().Engine.getWindow().getGLFWwindow(), 3.0f, 1.5f);
    }

    void GameObject::AddPointLightComponent()
    {
        this->addComponent<PointLightComponent>(glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f, 1.0f);
    }

    void GameObject::GUI_DisplayAddComponentMenu()
    {
        ImGui::SeparatorText("Add Component");

        std::string addComponentPopupIDName = "app_component";
        ImGuiID addComponentPopupID = ImGui::GetID(addComponentPopupIDName.c_str());

        if(ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup(addComponentPopupID);
        }

        if(ImGui::BeginPopup(addComponentPopupIDName.c_str()))
        {
            for(auto& selectable : _guiSelectables)
            {
                selectable.GUI_Render();
            }
            ImGui::EndPopup();
        }
    }

    void GameObject::GUI_DisplayAddMeshComponentMenu()
    {
        if(ImGui::BeginMenu("Mesh"))
        {
            ImGui::SeparatorText("Select a mesh file");
            if(ImGui::Selectable("Sitting"))
            {
                this->addComponent<MeshComponent>(_scene->getApp().Engine.getDevice(), "assets/Sitting.obj");
            }
            if(ImGui::Selectable("Floor"))
            {
                this->addComponent<MeshComponent>(_scene->getApp().Engine.getDevice(), "assets/Quad.obj");
            }
            ImGui::EndMenu();
        }
    }

    void GameObject::SetActive(bool isActive)
    {
        _isActive = isActive;
        for (auto& component : _components)
        {
            component->setActive(isActive);
        }
    }

    void GameObject::GUI_RenderComponentProperties()
    {
        for(auto& component : _components)
        {
            component->GUI_RenderProperties();
        }
 
        GUI_DisplayAddComponentMenu();        
    }
}
