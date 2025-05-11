#include "pch.h"
#include "default_scene_2.hpp"
#include "VGE_mesh.hpp"
#include "VGE_app.hpp"

namespace game
{
    DefaultScene2::DefaultScene2(VGE::VgeApp &app)
        : Scene(app)
    {
    }

    void DefaultScene2::Awake()
    {
        Scene::Awake();
        auto cameraObject = game::GameObject::createGameObjectPtr(this, "Camera");
        cameraObject->addComponent<game::CameraComponent>();
        cameraObject->addComponent<game::KeyboardController>(_app.Engine.getWindow().getGLFWwindow(), 3.0f, 1.5f);
        _cameraObject = cameraObject.get();
        _gameObjects.push_back(std::move(cameraObject));

        
    }

    void DefaultScene2::Update(float deltaTime)
    {
        float aspect = _app.Engine.getRenderer().getAspectRatio();
        _cameraObject->getComponent<game::CameraComponent>()->setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
    }

    void DefaultScene2::loadGameObjects()
    {
        std::shared_ptr<VGE::VgeMesh> mesh = VGE::VgeMesh::createModelFromFile(_app.Engine.getDevice(), "assets/Sitting.obj");
        auto go = game::GameObject::createGameObjectPtr(this);
        go->addComponent<game::MeshComponent>(_app.Engine.getDevice(), "assets/Sitting.obj");
        go->Transform->translation = {0.0f, 0.0f, 0.0f};
        go->Transform->scale = glm::vec3{0.2f};
        go->Transform->rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

        go = game::GameObject::createGameObjectPtr(this);
        go->addComponent<game::MeshComponent>(_app.Engine.getDevice(), "assets/Sitting.obj");
        go->Transform->translation = {1.0f, 0.0f, 0.0f};
        go->Transform->scale = glm::vec3{0.2f};
        go->Transform->rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

        go = game::GameObject::createGameObjectPtr(this);
        go->addComponent<game::MeshComponent>(_app.Engine.getDevice(), "assets/Sitting.obj");
        go->Transform->translation = {1.0f, 2.0f, 0.0f};
        go->Transform->scale = glm::vec3{0.2f};
        go->Transform->rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));
    }
}