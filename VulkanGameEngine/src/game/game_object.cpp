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
        _guiSelectables.push_back(GuiSelectable("Point Light Component",
            std::bind(&GameObject::AddPointLightComponent, this)));
        _guiSelectables.push_back(GuiSelectable("Keyboard Controller",
            std::bind(&GameObject::AddKBControllerComponent, this)));
    }

    void GameObject::AddKBControllerComponent()
    {
        this->addComponent<KeyboardController>(_scene->getApp().Engine.getWindow().getGLFWwindow(), 3.0f, 1.5f);
    }

    void GameObject::AddPointLightComponent()
    {
        this->addComponent<PointLightComponent>(glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f, 1.0f);
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
}
