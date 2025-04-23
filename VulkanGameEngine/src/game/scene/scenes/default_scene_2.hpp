#pragma once

#include "scene.hpp"

namespace game
{
    class DefaultScene2 : public Scene
    {
    public:
    DefaultScene2(VGE::VgeApp& app);
        virtual ~DefaultScene2() {};

        void Awake() override;
        void Update(float deltaTime) override;

    private:
        void loadGameObjects() override;

    private:
        
    };
}