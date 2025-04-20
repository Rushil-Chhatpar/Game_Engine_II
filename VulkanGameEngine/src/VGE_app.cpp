#include "pch.h"

#include "VGE_app.hpp"
#include "VGE_default_render_system.hpp"
#include "VGE_mesh.hpp"
#include "VGE_camera.hpp"
#include "keyboard_controller.hpp"
#include "VGE_buffer.hpp"
#include "VGE_frame_info.hpp"


namespace VGE
{
    struct GlobalUBO
    {
        alignas(16) glm::mat4 projectionViewMatrix{1.0f};
        alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, 1.0f, -1.0f});
    };

    VgeApp::VgeApp(VgeEngine& engine)
        : _engine(engine)
    {
        _globalPool = VgeDescriptorPool::Builder(_engine.getDevice())
            .setMaxSets(VgeSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VgeSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
            
        loadGameObjects();
    }

    VgeApp::~VgeApp()
    {
    }

    void VgeApp::run()
    {
        auto minOffsetAlignment = std::lcm(
            _engine.getDevice().properties.limits.minUniformBufferOffsetAlignment,
            _engine.getDevice().properties.limits.nonCoherentAtomSize);

        std::cout << "minOffsetAlignment: " << minOffsetAlignment << std::endl;

        std::vector<std::unique_ptr<VgeBuffer>> uboBuffers(VgeSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < VgeSwapChain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            uboBuffers[i] = std::make_unique<VgeBuffer>(_engine.getDevice(), sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        // create descriptor set layout
        auto globalSetLayout = VgeDescriptorSetLayout::Builder(_engine.getDevice())
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        // create descriptor sets
        std::vector<VkDescriptorSet> globalDescriptorSets(VgeSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < VgeSwapChain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkDescriptorBufferInfo bufferInfo = uboBuffers[i]->descriptorInfo();
            VgeDescriptorWriter(*globalSetLayout, *_globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        VgeDefaultRenderSystem renderSystem{_engine.getDevice(), _engine.getRenderer().getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        VgeCamera camera{};

        auto cameraObject = VgeGameObject::createGameObject();
        KeyboardController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!_engine.getWindow().shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(_engine.getWindow().getGLFWwindow(), deltaTime, cameraObject);
            camera.setViewYXZ(cameraObject.Transform.translation, cameraObject.Transform.rotation);

            float aspect = _engine.getRenderer().getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

            if (VkCommandBuffer commandBuffer = _engine.getRenderer().beginFrame())
            {
                int frameIndex = _engine.getRenderer().getFrameIndex();

                // update objects in memory
                GlobalUBO ubo{};
                ubo.projectionViewMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // This level of separation is necessary
                // It can easily integrate multiple render passes for things like
                // shadow mapping, post processing, reflections, etc.
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass
                FrameInfo frameInfo{
                    .frameIndex = frameIndex,
                    .frameTime = deltaTime,
                    .commandBuffer = commandBuffer,
                    .camera = camera,
                    .globalDescriptorSet = globalDescriptorSets[frameIndex],
                };
                _engine.getRenderer().beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(frameInfo, _gameObjects);
                _engine.getRenderer().endSwapChainRenderPass(commandBuffer);
                _engine.getRenderer().endFrame();
            }
        }

        vkDeviceWaitIdle(_engine.getDevice().device());
    }

    void VgeApp::loadGameObjects()
    {
        // std::shared_ptr<VgeMesh> mesh = createCubeModel(_engine.getDevice(), {0.0f, 0.0f, 0.0f});
        // auto cube = VgeGameObject::createGameObject();
        // cube.setMesh(mesh);
        // cube.Transform.translation = {0.0f, 0.0f, 2.5f};
        // cube.Transform.scale = {0.5f, 0.5f, 0.5f};

        std::shared_ptr<VgeMesh> mesh = VgeMesh::createModelFromFile(_engine.getDevice(), "../assets/Sitting.obj");
        auto go = VgeGameObject::createGameObject();
        go.setMesh(mesh);
        go.Transform.translation = {0.0f, 0.0f, 0.0f};
        go.Transform.scale = glm::vec3{0.2f};
        go.Transform.rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

        go = VgeGameObject::createGameObject();
        go.setMesh(mesh);
        go.Transform.translation = {2.0f, 0.0f, 0.0f};
        go.Transform.scale = glm::vec3{0.2f};
        go.Transform.rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

    }

    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<VgeMesh> VgeApp::createCubeModel(VgeDevice& device, glm::vec3 offset) 
    {
        VgeMesh::MeshBuilder builder;
        builder.vertices = 
        {
   
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            
        };
        for (auto& v : builder.vertices) 
        {
            v.pos += offset;
        }

        builder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
        return std::make_unique<VgeMesh>(device, builder);
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
    //     std::shared_ptr<VgeMesh> mesh = std::make_shared<VgeMesh>(_engine.getDevice(), vertices);

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