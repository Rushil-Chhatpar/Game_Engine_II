#pragma once

namespace game
{
    class Component;
    class ComponentManager
    {
        DECLARE_SINGLETON(ComponentManager);
    public:

        void Register(Component* component);
        void Unregister(Component* component);
        void Awake();
        void Update(float deltaTime);

    protected:
        // non owning reference pointers
        std::vector<Component*> _components;
    };
}