#pragma once

namespace game
{

    class Scene;

    class SceneManager
    {
        DECLARE_SINGLETON(SceneManager);

    public:
        void Update();
        void Render();
        

    private:
        std::shared_ptr<Scene> _activeScene;

    };
}