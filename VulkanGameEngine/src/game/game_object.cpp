#include "pch.h"
#include "game_object.hpp"

namespace game
{
    void GameObject::SetActive(bool isActive)
    {
        _isActive = isActive;
        for (auto& component : _components)
        {
            component->setActive(isActive);
        }
    }

    void GameObject::GUI_RenderComponentProperties()
    {
        for(auto& component : _components)
        {
            component->GUI_RenderProperties();
        }
    }
}
