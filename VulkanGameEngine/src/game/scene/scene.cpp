#include "pch.h"

#include "scene.hpp"
#include "VGE_app.hpp"
#include "VGE_default_render_system.hpp"
#include "VGE_pointlight_render_system.hpp"
#include "VGE_frame_info.hpp"
#include "VGE_mesh.hpp"
#include "gui_manager.hpp"


namespace game
{
    void Scene::Awake()
    {
        loadGameObjects();
        
        _guiSelectables.push_back(GuiSelectable("Create Empty", 
            std::bind(&Scene::createEmptyGameObject, this)));
        _guiSelectables.push_back(GuiSelectable("Create Point Light",
            std::bind(&Scene::createPointLightGameObject, this)));
    }

    void Scene::Update(float deltaTime)
    {
    }

    void Scene::Render(VGE::FrameInfo& frameInfo, VGE::VgeDefaultRenderSystem* renderSystem, VGE::VgePointLightRenderSystem* pointLightRenderSystem, VGE::GlobalUBO& ubo)
    {
        // TODO: I NEED A BETTER WAY TO DO THIS
        std::vector<GameObject*> lights;

        for (auto& gameObject : _gameObjects)
        {
            if (gameObject->isActive() && gameObject->getComponent<PointLightComponent>() != nullptr)
            {
                lights.push_back(gameObject.get());
            }
        }

        pointLightRenderSystem->update(frameInfo, ubo, lights);

        pointLightRenderSystem->render(frameInfo, lights);

        std::vector<GameObject*> renderables;
        renderables.reserve(_gameObjects.size());

        for (auto& gameObject : _gameObjects)
        {
            if (gameObject->isActive() && gameObject->getComponent<MeshComponent>() != nullptr)
            {
                renderables.push_back(gameObject.get());
            }
        }

        renderSystem->renderGameObjects(frameInfo, renderables);
        //renderSystem.renderGameObjectsinBatch(frameInfo, renderables);
    }

    void Scene::LoadMeshesOnRenderSystem(VGE::VgeDefaultRenderSystem* renderSystem)
    {
        std::vector<GameObject*> renderables;
        renderables.reserve(_gameObjects.size());

        for (auto& gameObject : _gameObjects)
        {
            if (gameObject->isActive() && gameObject->getComponent<MeshComponent>() != nullptr)
            {
                renderables.push_back(gameObject.get());
            }
        }

        renderSystem->createBatchBuffers(renderables);
    }

    void Scene::Activate()
    {
        _isActive = true;
        for(auto& gameObject : _gameObjects)
        {
            gameObject->SetActive(true);
        }
    }

    void Scene::Deactivate()
    {
        _isActive = false;
        for(auto& gameObject : _gameObjects)
        {
            gameObject->SetActive(false);
        }
    }

    void Scene::GUI_DisplayObjectList()
    {
        GUI_DisplayCreateObjectMenu();

        ImGui::SeparatorText("GameObjects");

        for(auto& gameObject : _gameObjects)
        {
            uint8_t flags = 0;
            bool isSelected = gameObject.get() == _guiSelectedObject;
            
            if(ImGui::Selectable(gameObject->getName().c_str(), &isSelected, flags))
            {
                _guiSelectedObject = gameObject.get();
            }
        }
    }

    void Scene::GUI_DisplayObjectProperties()
    {
        if(_guiSelectedObject)
        {
            ImGui::SeparatorText(_guiSelectedObject->getName().c_str());
            _guiSelectedObject->GUI_RenderComponentProperties();
        }
    }

    void Scene::GUI_DisplayCreateObjectMenu()
    {
        std::string createGOPopupIDName = "create_gameobject";
        ImGuiID createGOPopupID = ImGui::GetID(createGOPopupIDName.c_str());

        if(ImGui::Button("Create GameObject"))
        {
            ImGui::OpenPopup(createGOPopupID);
        }

        if(ImGui::BeginPopup(createGOPopupIDName.c_str()))
        {
            for(auto& selectable : _guiSelectables)
            {
                selectable.GUI_Render();
            }
            ImGui::EndPopup();
        }
    }

    void Scene::createEmptyGameObject()
    {
        auto go = GameObject::createGameObjectPtr(this);
        go->Transform->translation = {0.0f, 0.0f, 0.0f};
        go->Transform->scale = glm::vec3{1.0f};
        go->Transform->rotation = {0.0f, 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));
        _guiSelectedObject = _gameObjects.back().get();
    }

    void Scene::createPointLightGameObject()
    {
        auto go = GameObject::createGameObjectPtr(this, "Point Light");
        go->addComponent<PointLightComponent>(glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f, 1.0f);
        go->Transform->translation = {0.0f, 0.0f, 0.0f};
        go->Transform->scale = glm::vec3{1.0f};
        go->Transform->rotation = {0.0f, 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));
        _guiSelectedObject = _gameObjects.back().get();
    }
}

/*
            if (ImGui::TreeNode(gameObject->getName().c_str()))
            {
                ImGui::Text("Position: %.2f, %.2f, %.2f", gameObject->Transform->translation.x, gameObject->Transform->translation.y, gameObject->Transform->translation.z);
                ImGui::Text("Rotation: %.2f, %.2f, %.2f", gameObject->Transform->rotation.x, gameObject->Transform->rotation.y, gameObject->Transform->rotation.z);
                ImGui::Text("Scale: %.2f, %.2f, %.2f", gameObject->Transform->scale.x, gameObject->Transform->scale.y, gameObject->Transform->scale.z);
                ImGui::TreePop();
            }
*/