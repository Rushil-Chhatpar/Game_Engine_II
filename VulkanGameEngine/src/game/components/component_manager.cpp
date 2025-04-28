#include "pch.h"
#include "component_manager.hpp"
#include "component.hpp"

namespace game
{
    DEFINE_SINGLETON(ComponentManager);

    
    void ComponentManager::Register(Component* component)
    {
        _components.push_back(component);
    }

    void ComponentManager::Unregister(Component* component)
    {
        auto it = std::find(_components.begin(), _components.end(), component);
        if (it != _components.end())
        {
            _components.erase(it);
        }
    }

    void ComponentManager::Awake()
    {
        for (auto& component : _components)
        {
            component->awake();
        }
    }

    void ComponentManager::Update(float deltaTime)
    {
        for (auto& component : _components)
        {
            if(component->isActive())
                component->update(deltaTime);
        }
    }
}