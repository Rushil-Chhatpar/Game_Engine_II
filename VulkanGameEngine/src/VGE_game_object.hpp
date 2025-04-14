#pragma once

namespace VGE
{
    class VgeGameObject
    {
    public:
        VgeGameObject();
        ~VgeGameObject();

        VgeGameObject(const VgeGameObject&) = delete;
        VgeGameObject& operator=(const VgeGameObject&) = delete;
        
    };
}