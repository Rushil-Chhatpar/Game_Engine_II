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

    void SceneManager::LoadMeshesOnRenderSystem(VGE::VgeDefaultRenderSystem &renderSystem)
    {
        for(const auto& scene : _scenes)
        {
            scene->LoadMeshesOnRenderSystem(renderSystem);
        }
    }

    void SceneManager::GUI_DisplayObjectList()
    {
        if (_activeScene)
        {
            _activeScene->GUI_DisplayObjectList();
        }
    }

    void SceneManager::GUI_DisplayObjectProperties()
    {
        if (_activeScene)
        {
            _activeScene->GUI_DisplayObjectProperties();
        }        
    }
}