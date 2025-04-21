#pragma once


namespace game
{
    class GameObject;

    class Component
    {
    public:
        Component(GameObject& owner);
        virtual ~Component() = default;
        virtual void awake() = 0;
        virtual void update(float deltaTime) = 0;

    private:
        GameObject& _owner;
    };
}