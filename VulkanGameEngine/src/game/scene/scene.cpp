#include "pch.h"

#include "scene.hpp"
#include "VGE_app.hpp"
#include "VGE_default_render_system.hpp"
#include "VGE_frame_info.hpp"
#include "VGE_mesh.hpp"


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
}