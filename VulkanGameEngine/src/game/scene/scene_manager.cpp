#include "pch.h"

#include "scene_manager.hpp"
#include "scene.hpp"
#include "VGE_default_render_system.hpp"
#include "VGE_frame_info.hpp"


namespace game
{
    DEFINE_SINGLETON(SceneManager);

    
    void SceneManager::Awake()
    {
        for(const auto& scene : _scenes)
        {
            scene->Awake();
        }
    }

    void SceneManager::Update(float deltaTime)
    {
        if (_activeScene)
        {
            _activeScene->Update(deltaTime);
        }
    }

    void SceneManager::Render(VGE::FrameInfo& frameInfo, VGE::VgeDefaultRenderSystem& renderSystem)
    {
        if (_activeScene)
        {
            _activeScene->Render(frameInfo, renderSystem);
        }
    }
}