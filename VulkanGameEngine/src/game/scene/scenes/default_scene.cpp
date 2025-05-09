#include "pch.h"
#include "default_scene.hpp"
#include "VGE_mesh.hpp"
#include "VGE_app.hpp"

namespace game
{
    DefaultScene::DefaultScene(VGE::VgeApp &app)
        : Scene(app)
    {
    }

    void DefaultScene::Awake()
    {
        Scene::Awake();
        auto cameraObject = game::GameObject::createGameObjectPtr("Camera");
        cameraObject->addComponent<game::CameraComponent>();
        cameraObject->addComponent<game::KeyboardController>(_app.Engine.getWindow().getGLFWwindow(), 3.0f, 1.5f);
        _cameraObject = cameraObject.get();
        _gameObjects.push_back(std::move(cameraObject));
    }

    void DefaultScene::Update(float deltaTime)
    {
        float aspect = _app.Engine.getRenderer().getAspectRatio();
        _cameraObject->getComponent<game::CameraComponent>()->setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
    }

    void DefaultScene::loadGameObjects()
    {
        std::shared_ptr<VGE::VgeMesh> mesh = VGE::VgeMesh::createModelFromFile(_app.Engine.getDevice(), "assets/Sitting.obj");
        auto go = game::GameObject::createGameObjectPtr();
        go->addComponent<game::MeshComponent>(_app.Engine.getDevice(), "assets/Sitting.obj");
        go->Transform->translation = {0.0f, 0.0f, 0.0f};
        go->Transform->scale = glm::vec3{0.2f};
        go->Transform->rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

        go = game::GameObject::createGameObjectPtr();
        go->addComponent<game::MeshComponent>(_app.Engine.getDevice(), "assets/Sitting.obj");
        go->Transform->translation = {2.0f, 0.0f, 0.0f};
        go->Transform->scale = glm::vec3{0.2f};
        go->Transform->rotation = {glm::pi<float>(), 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

        go = game::GameObject::createGameObjectPtr("Floor");
        go->addComponent<game::MeshComponent>(_app.Engine.getDevice(), "assets/Quad.obj");
        go->Transform->translation = {0.0f, 0.5f, 0.0f};
        go->Transform->scale = glm::vec3{2.0f};
        go->Transform->rotation = {0.0f, 0.0f, 0.0f};
        _gameObjects.push_back(std::move(go));

        go = game::GameObject::createGameObjectPtr("Light");
        go->addComponent<game::PointLightComponent>(glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f, 1.0f);
        _gameObjects.push_back(std::move(go));
    }
}