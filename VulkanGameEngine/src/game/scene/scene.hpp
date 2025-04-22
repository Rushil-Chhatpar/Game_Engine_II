#pragma once

#include "game_object.hpp"

namespace VGE
{
    class VgeApp;
    class VgeDefaultRenderSystem;
    struct FrameInfo;
}


namespace game
{
    class GameObject;
    class Scene
    {
    public:
        Scene(VGE::VgeApp& app) : _app(app) {};
        virtual ~Scene() {};

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        virtual void Awake();
        virtual void Update(float deltaTime);
        virtual void Render(VGE::FrameInfo& frameInfo, VGE::VgeDefaultRenderSystem& renderSystem);

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
    };
}