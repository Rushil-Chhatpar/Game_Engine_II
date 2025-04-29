#pragma once


namespace VGE
{
    class VgeApp;
    class VgeDefaultRenderSystem;
    struct FrameInfo;
}

namespace game
{

    class Scene;

    class SceneManager
    {
        DECLARE_SINGLETON(SceneManager);

    public:
        void Awake();
        void Update(float deltaTime);
        void Render(VGE::FrameInfo& frameInfo, VGE::VgeDefaultRenderSystem& renderSystem);
        void LoadMeshesOnRenderSystem(VGE::VgeDefaultRenderSystem& renderSystem);

        void RenderGUIonActiveScene();
        
        
        // get active scene
        Scene* getActiveScene() const
        {
            return _activeScene;
        }

        template<typename T, typename... Args>
        T* makeScene(VGE::VgeApp& app, Args&&... args)
        {
            static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
            static_assert(!std::is_same<T, Scene>::value, "T cannot be Scene itself");
            // Check if the scene already exists
            if (getScene<T>() != nullptr)
            {
                return nullptr; // or throw an exception
            }
            std::unique_ptr<T> scene = std::make_unique<T>(app, std::forward<Args>(args)...);
            T* ptr = scene.get();
            _scenes.emplace_back(std::move(scene));
            if(_activeScene)
                _activeScene->Deactivate();
            _activeScene = ptr;
            return ptr;
        }

        template<typename T>
        T* getScene()
        {
            for (const auto& scene : _scenes)
            {
                if (T* casted = dynamic_cast<T*>(scene.get()))
                {
                    return casted;
                }
            }
            return nullptr;
        }

        template<typename T>
        void setActiveScene()
        {
            for (const auto& scene : _scenes)
            {
                if (T* casted = dynamic_cast<T*>(scene.get()))
                {
                    _activeScene->Deactivate();
                    _activeScene = casted;
                    _activeScene->Activate();
                    return;
                }
            }
        }

    private:
        std::vector<std::unique_ptr<Scene>> _scenes;
        Scene* _activeScene = nullptr;

    };
}