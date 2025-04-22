#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "VGE_window.hpp"

namespace VGE
{
    class VgeDevice;
    class VgeMesh;
}
namespace game
{
    class GameObject;

    class Component
    {
    public:
        Component(GameObject& owner);
        virtual ~Component() = default;
        virtual void awake() = 0;
        virtual void update(float deltaTime) = 0;

    protected:
        GameObject& _owner;
    };

    class TransformComponent : public Component
    {
    public:
        TransformComponent(GameObject& owner) : Component(owner) {}
        ~TransformComponent() override = default;

        glm::mat4 mat4();
        glm::mat3 normalMatrix();

        virtual void awake() override {}
        virtual void update(float deltaTime) override {}

        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{};
    };

    class CameraComponent : public Component
    {
    public:
        CameraComponent(GameObject& owner) : Component(owner) {}
        ~CameraComponent() override = default;

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovY, float aspect, float near, float far);
        
        /*
        * @brief Sets the view matrix using a direction vector and an up vector.
        * @param position The position of the camera.
        * @param direction The direction the camera is facing.
        * @param up The up vector of the camera. Default is (0, -1, 0).
        * @note After using the values, it constructs an Orthonormal basis
        *       and sets the view matrix accordingly.
        * @note The Orthonormal basis is used to construct a rotation matrix
        * and then combined with the translation matrix to set the view matrix.
        * @note The Rotation Matrix:
        * @note | u.x  v.x  w.x  0 |
        * @note | u.y  v.y  w.y  0 |
        * @note | u.z  v.z  w.z  0 |
        * @note | 0    0    0    1 |
        * @note The Translation Matrix:
        * @note | 1  0  0  -position.x |
        * @note | 0  1  0  -position.y |
        * @note | 0  0  1  -position.z |
        * @note | 0  0  0       1      |
        */
        void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
        void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);
        void setViewYXZ();
        
        const glm::mat4& getProjectionMatrix() const { return _projectionMatrix; }
        const glm::mat4& getViewMatrix() const { return _viewMatrix; }

        virtual void awake() override {}
        virtual void update(float deltaTime) override
        {
            setViewYXZ();
        }
    private:
        glm::mat4 _projectionMatrix{1.0f};
        glm::mat4 _viewMatrix{1.0f};       
    };

    class KeyboardController : public Component
    {
    public:
        struct KeyMappings
        {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        KeyboardController(GameObject& owner, GLFWwindow* window, float moveSpeed, float lookSpeed) 
            : Component(owner)
            , _window(window)
            , _moveSpeed(moveSpeed)
            , _lookSpeed(lookSpeed)
        {}
        ~KeyboardController() override = default;

        virtual void awake() override {}
        virtual void update(float deltaTime) override;
        
        
        private:
        void moveInPlaneXZ(float deltaTime);
        
        
    private:
        GLFWwindow* _window;
        KeyMappings Keys{};
        float _moveSpeed{3.0f};
        float _lookSpeed{1.5f};
    };

    class MeshComponent : public Component
    {
    public:
        MeshComponent(GameObject& owner, VGE::VgeDevice& device, const char *filename);
        ~MeshComponent() override = default;

        virtual void awake() override {}
        virtual void update(float deltaTime) override {}

        VGE::VgeMesh* getMesh() const { return _mesh.get(); }

    private:
        std::unique_ptr<VGE::VgeMesh> _mesh;
    };

}