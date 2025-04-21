#include "pch.h"
#include "component.hpp"

#include "game_object.hpp"

namespace game
{
    Component::Component(GameObject& owner)
        : _owner(owner)
    {
    }
}