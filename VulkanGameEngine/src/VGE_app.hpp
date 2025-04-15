#pragma once

#include "VGE_device.hpp"
#include "VGE_window.hpp"
#include "VGE_renderer.hpp"
#include "VGE_game_object.hpp"

#include <memory>
#include <vector>

namespace VGE
{
    class VgeApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 800;

        VgeApp();
        ~VgeApp();

        VgeApp(const VgeApp &) = delete;
        VgeApp& operator=(const VgeApp &) = delete;

        void run();

    private:
    
        void loadGameObjects();


        std::unique_ptr<VgeMesh> createCubeModel(VgeDevice& device, glm::vec3 offset);

    private:
        VgeWindow _window{WIDTH, HEIGHT, "Vulkan Game Engine"};
        VgeDevice _device{_window};
        VgeRenderer _renderer{_window, _device};

        std::vector<VgeGameObject> _gameObjects;

        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
    };
}