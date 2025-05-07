#include "pch.h"

#include "scene.hpp"
#include "VGE_app.hpp"
#include "VGE_default_render_system.hpp"
#include "VGE_frame_info.hpp"
#include "VGE_mesh.hpp"
#include "gui_manager.hpp"


namespace game
{
    void Scene::Awake()
    {
        loadGameObjects();
    }

    void Scene::Update(float deltaTime)
    {
    }

    void Scene::Render(VGE::FrameInfo& frameInfo, VGE::VgeDefaultRenderSystem& renderSystem)
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

        renderSystem.renderGameObjects(frameInfo, renderables);
        //renderSystem.renderGameObjectsinBatch(frameInfo, renderables);
    }

    void Scene::LoadMeshesOnRenderSystem(VGE::VgeDefaultRenderSystem &renderSystem)
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

        renderSystem.createBatchBuffers(renderables);
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
            ImGui::LabelText("Name", "%s", _guiSelectedObject->getName().c_str());
            _guiSelectedObject->GUI_RenderComponentProperties();
        }
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