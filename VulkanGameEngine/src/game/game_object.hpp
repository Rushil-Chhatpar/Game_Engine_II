#pragma once

#include "VGE_mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "component_manager.hpp"
#include "component.hpp"

#include <memory>

namespace game
{
    class Scene;
    struct GuiSelectable;

    class GameObject
    {
    public:
        using id_t = unsigned int;
        
        // Factory method to create a new game object
        static GameObject createGameObject(Scene* scene, std::string name = "GameObject")
        {
            static id_t currentId = 0;
            currentId++;
            GameObject obj(currentId, std::to_string(currentId) + ". " + name, scene);
            obj.Transform = obj.addComponent<TransformComponent>();

            return obj;
        }

        // Factory method to create a new game object with a unique pointer
        // This is useful for managing the lifetime of the game object
        // and its components automatically
        static std::unique_ptr<GameObject> createGameObjectPtr(Scene* scene, std::string name = "GameObject")
        {
            static id_t currentId = 0;
            currentId++;
            std::unique_ptr<GameObject> obj(new GameObject(currentId, std::to_string(currentId) + ". " + name, scene));
            obj->Transform = obj->addComponent<TransformComponent>();

            return obj;
        }
        
        // Get the ID of the game object
        id_t getId() const { return _id; }

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;

        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        void SetActive(bool isActive);

        void GUI_RenderComponentProperties();

        // setters
        void setActive(bool isActive) { _isActive = isActive; }
        
        // getters
        bool isActive() const { return _isActive; }
        const std::string& getName() const { return _name; }

        // Components
        template<typename T, typename... Args>
        T* addComponent(Args&&... args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a derived class of Component");
            static_assert(!std::is_same<T, Component>::value, "T cannot be Component itself");
            
            // Check if the component already exists
            if (getComponent<T>() != nullptr)
            {
                return nullptr; // or throw an exception
            }

            std::unique_ptr<T> component = std::make_unique<T>(*this, std::forward<Args>(args)...);
            T* ptr = component.get();
            _components.emplace_back(std::move(component));
            GET_SINGLETON(ComponentManager)->Register(ptr);
            return ptr;
        }

        template<typename T>
        T* getComponent()
        {
            for (const auto& component : _components)
            {
                if (T* casted = dynamic_cast<T*>(component.get()))
                {
                    return casted;
                }
            }
            return nullptr;
        }

        template<typename T>
        void removeComponent()
        {
            auto it = std::remove_if(_components.begin(), _components.end(),
                [](const std::unique_ptr<Component>& component) { return dynamic_cast<T*>(component.get()) != nullptr; });
            
            if(it == _components.end())
            {
                return; // Component not found
            }
            // Unregister the component from the ComponentManager
            GET_SINGLETON(ComponentManager)->Unregister(it->get());
            // Remove the component from the vector
            _components.erase(it, _components.end());
        }
        
    private:
        GameObject(id_t id, std::string name, Scene* scene);

        void AddKBControllerComponent();
        void AddPointLightComponent();
        void AddMeshComponent(const char* filename);

        void GUI_DisplayAddMeshComponentMenu();

        
    public:
        TransformComponent* Transform = nullptr;
    private:
        id_t _id;
        std::string _name;
        Scene* _scene = nullptr;
        
        bool _isActive = true;

        
        // Components
        std::vector<std::unique_ptr<Component>> _components;

        std::vector<GuiSelectable> _guiSelectables;
    };
}