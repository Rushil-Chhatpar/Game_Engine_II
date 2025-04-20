#include "pch.h"
#include "VGE_engine.hpp"

namespace VGE
{
    VgeEngine::VgeEngine(int w, int h, std::string name)
    {
        _window = std::make_shared<VgeWindow>(w, h, name);
        _device = std::make_shared<VgeDevice>(*_window);
        _renderer = std::make_shared<VgeRenderer>(*_window, *_device);
    }

    VgeEngine::~VgeEngine()
    {
    }
}
