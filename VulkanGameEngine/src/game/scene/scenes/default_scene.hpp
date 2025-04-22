#pragma once

#include "scene.hpp"

namespace game
{
    class DefaultScene : public Scene
    {
    public:
        DefaultScene(VGE::VgeApp& app);
        virtual ~DefaultScene() {};

        void Awake() override;
        void Update(float deltaTime) override;

    private:
        void loadGameObjects() override;

    private:
        
    };
}