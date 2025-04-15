#include "VGE_app.hpp"
#include "VGE_default_render_system.hpp"
#include "VGE_mesh.hpp"
#include "VGE_camera.hpp"


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
        VgeCamera camera{};
        while (!_window.shouldClose())
        {
            glfwPollEvents();

            float aspect = _renderer.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

            if (VkCommandBuffer commandBuffer = _renderer.beginFrame())
            {
                // This level of separation is necessary
                // It can easily integrate multiple render passes for things like
                // shadow mapping, post processing, reflections, etc.
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass
                _renderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, _gameObjects, camera);
                _renderer.endSwapChainRenderPass(commandBuffer);
                _renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(_device.device());
    }

    void VgeApp::loadGameObjects()
    {
        std::shared_ptr<VgeMesh> mesh = createCubeModel(_device, {0.0f, 0.0f, 0.0f});
        auto cube = VgeGameObject::createGameObject();
        cube.setMesh(mesh);
        cube.Transform.translation = {0.0f, 0.0f, 2.5f};
        cube.Transform.scale = {0.5f, 0.5f, 0.5f};
        _gameObjects.push_back(std::move(cube));
    }

    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<VgeMesh> VgeApp::createCubeModel(VgeDevice& device, glm::vec3 offset) 
    {
        std::vector<VgeMesh::Vertex> vertices
        {
   
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
   
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
   
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
   
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            
        };
        for (auto& v : vertices) 
        {
            v.pos += offset;
        }
        return std::make_unique<VgeMesh>(device, vertices);
    }
}


















// DEPRECATED CODE
    // void VgeApp::createFlowerPattern()
    // {
    //     std::vector<VgeMesh::Vertex> vertices{
    //         {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    //         {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    //         {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    //     };
        
    //     VgeMesh::sierpinskiTriangle(vertices, 5, {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.f, 0.5f});
    //     std::shared_ptr<VgeMesh> mesh = std::make_shared<VgeMesh>(_device, vertices);

    //     std::vector<glm::vec3> colors
    //     {
    //         {1.f, .7f, .73f},
    //         {1.f, .87f, .73f},
    //         {1.f, 1.f, .73f},
    //         {.73f, 1.f, .8f},
    //         {.73, .88f, 1.f}  
    //     };
    //     for (auto& color : colors) 
    //     {
    //         color = glm::pow(color, glm::vec3{2.2f});
    //     }

    //     for (int i = 0; i < 40; i++)
    //     {
    //         auto triangle = VgeGameObject::createGameObject();
    //         triangle.setMesh(mesh);
    //         triangle.Transform.scale = glm::vec2(.5f) + i * 0.025f;
    //         triangle.Transform.rotation = i * glm::pi<float>() * .025f;
    //         triangle.setColor(colors[i % colors.size()]);
    //         _gameObjects.push_back(std::move(triangle));
    //     }
    // }