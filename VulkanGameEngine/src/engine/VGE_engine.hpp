#pragma once

#include <VGE_device.hpp>
#include <VGE_renderer.hpp>
#include <VGE_window.hpp>

#include <memory>

namespace VGE
{

    class VgeEngine
    {
    public:
        VgeEngine(int w, int h, std::string name);
        ~VgeEngine();

        VgeEngine(const VgeEngine &) = delete;
        VgeEngine& operator=(const VgeEngine &) = delete;

        // getters
        VgeWindow& getWindow() { return *_window.get(); }
        VgeDevice& getDevice() { return *_device.get(); }
        VgeRenderer& getRenderer() { return *_renderer.get(); }

    private:    
        std::shared_ptr<VgeWindow> _window;
        std::shared_ptr<VgeDevice> _device;
        std::shared_ptr<VgeRenderer> _renderer;

        
    };

}