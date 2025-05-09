#pragma once

#include "game_object.hpp"

namespace VGE
{
    class VgeApp;
    class VgeDefaultRenderSystem;
    class VgePointLightRenderSystem;
    struct FrameInfo;
    struct GlobalUBO;
}


namespace game
{
    class GameObject;
    class Scene
    {
        friend class SceneManager;
    public:
        Scene(VGE::VgeApp& app) : _app(app) {};
        virtual ~Scene() {};

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        virtual void Awake();
        virtual void Update(float deltaTime);
        virtual void Render(VGE::FrameInfo& frameInfo, VGE::VgeDefaultRenderSystem* renderSystem, VGE::VgePointLightRenderSystem* pointLightRenderSystem, VGE::GlobalUBO& ubo);
        virtual void LoadMeshesOnRenderSystem(VGE::VgeDefaultRenderSystem* renderSystem);

        void Activate();
        void Deactivate();

        void GUI_DisplayObjectList();
        void GUI_DisplayObjectProperties();

        CameraComponent* getCamera() const
        {
            return _cameraObject ? _cameraObject->getComponent<CameraComponent>() : nullptr;
        }

    protected:
        virtual void loadGameObjects() = 0;

    protected:
        VGE::VgeApp& _app;

        std::vector<std::unique_ptr<GameObject>> _gameObjects;
        GameObject* _cameraObject = nullptr;
        GameObject* _guiSelectedObject = nullptr;
        bool _isActive = true;
    };
}