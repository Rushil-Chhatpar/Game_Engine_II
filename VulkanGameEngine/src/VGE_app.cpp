#include "VGE_app.hpp"
#include "VGE_default_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <iostream>
#include <array>

namespace VGE
{
    VgeApp::VgeApp()
    {
        loadGameObjects();
    }

    VgeApp::~VgeApp()
    {
    }

    void VgeApp::run()
    {
        VgeDefaultRenderSystem renderSystem{_device, _renderer.getSwapChainRenderPass()};
        while (!_window.shouldClose())
        {
            glfwPollEvents();
            if (VkCommandBuffer commandBuffer = _renderer.beginFrame())
            {
                // This level of separation is necessary
                // It can easily integrate multiple render passes for things like
                // shadow mapping, post processing, reflections, etc.
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass
                _renderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, _gameObjects);
                _renderer.endSwapChainRenderPass(commandBuffer);
                _renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(_device.device());
    }

    void VgeApp::loadGameObjects()
    {
        std::vector<VgeMesh::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        
        VgeMesh::sierpinskiTriangle(vertices, 5, {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.f, 0.5f});
        std::shared_ptr<VgeMesh> mesh = std::make_shared<VgeMesh>(_device, vertices);

        std::vector<glm::vec3> colors
        {
            {1.f, .7f, .73f},
            {1.f, .87f, .73f},
            {1.f, 1.f, .73f},
            {.73f, 1.f, .8f},
            {.73, .88f, 1.f}  //
        };
        for (auto& color : colors) 
        {
            color = glm::pow(color, glm::vec3{2.2f});
        }

        for (int i = 0; i < 40; i++)
        {
            auto triangle = VgeGameObject::createGameObject();
            triangle.setMesh(mesh);
            triangle.Transform2d.scale = glm::vec2(.5f) + i * 0.025f;
            triangle.Transform2d.rotation = i * glm::pi<float>() * .025f;
            triangle.setColor(colors[i % colors.size()]);
            _gameObjects.push_back(std::move(triangle));
        }
    }
}