#pragma once

#include "VGE_device.hpp"
#include "VGE_window.hpp"
#include "VGE_renderer.hpp"
#include "game_object.hpp"
#include "VGE_descriptors.hpp"
#include <VGE_engine.hpp>

#include <memory>
#include <vector>

namespace VGE
{
    class VgeApp
    {
    public:
        static constexpr int WIDTH = 1280;
        static constexpr int HEIGHT = 720;

        VgeApp(VgeEngine& engine);
        ~VgeApp();

        VgeApp(const VgeApp &) = delete;
        VgeApp& operator=(const VgeApp &) = delete;

        void run();

        private:        
        
        void drawAppUI();
        std::unique_ptr<VgeMesh> createCubeModel(VgeDevice& device, glm::vec3 offset);
        
    public:
        VgeEngine& Engine;
        
    private: 
        VkClearColorValue _defaultClearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
        
        std::unique_ptr<VgeDescriptorPool> _globalPool;
        std::vector<game::GameObject> _gameObjects;
    };
}