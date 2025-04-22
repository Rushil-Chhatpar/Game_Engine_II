#include "pch.h"
#include "component.hpp"

#include "VGE_device.hpp"
#include "game_object.hpp"
#include "VGE_mesh.hpp"

namespace game
{
    Component::Component(GameObject& owner)
        : _owner(owner)
    {
    }

    glm::mat4 TransformComponent::mat4()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4
        {
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {translation.x, translation.y, translation.z, 1.0f}
        };
    }

    glm::mat3 TransformComponent::normalMatrix()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 invScale = 1.0f/scale;
        return glm::mat3
        {
            {
                invScale.x * (c1 * c3 + s1 * s2 * s3),
                invScale.x * (c2 * s3),
                invScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                invScale.y * (c3 * s1 * s2 - c1 * s3),
                invScale.y * (c2 * c3),
                invScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                invScale.z * (c2 * s1),
                invScale.z * (-s2),
                invScale.z * (c1 * c2),
            },
        };
    }

    void CameraComponent::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        _projectionMatrix[0][0] = 2.0f / (right - left);
        _projectionMatrix[1][1] = 2.0f / (bottom - top);
        _projectionMatrix[2][2] = 1.0f / (far - near);
        _projectionMatrix[3][0] = -(right + left) / (right - left);
        _projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        _projectionMatrix[3][2] = -near / (far - near);
    }

    void CameraComponent::setPerspectiveProjection(float fovY, float aspect, float near, float far)
    {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f && "Aspect ratio cannot be zero");
        const float tanHalfFovY = tan(fovY / 2.f);
        _projectionMatrix = glm::mat4(0.0f);
        _projectionMatrix[0][0] = 1.0f / (aspect * tanHalfFovY);
        _projectionMatrix[1][1] = 1.0f / (tanHalfFovY);
        _projectionMatrix[2][2] = far / (far - near);
        _projectionMatrix[2][3] = 1.0f;
        _projectionMatrix[3][2] = -(far * near) / (far - near);
    }

    void CameraComponent::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
    {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};
      
        _viewMatrix = glm::mat4{1.f};
        _viewMatrix[0][0] = u.x;
        _viewMatrix[1][0] = u.y;
        _viewMatrix[2][0] = u.z;
        _viewMatrix[0][1] = v.x;
        _viewMatrix[1][1] = v.y;
        _viewMatrix[2][1] = v.z;
        _viewMatrix[0][2] = w.x;
        _viewMatrix[1][2] = w.y;
        _viewMatrix[2][2] = w.z;
        _viewMatrix[3][0] = -glm::dot(u, position);
        _viewMatrix[3][1] = -glm::dot(v, position);
        _viewMatrix[3][2] = -glm::dot(w, position);
    }

    void CameraComponent::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    {
        setViewDirection(position, target - position, up);
    }

    void CameraComponent::setViewYXZ(glm::vec3 position, glm::vec3 rotation)
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
        const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
        const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
        _viewMatrix = glm::mat4{1.f};
        _viewMatrix[0][0] = u.x;
        _viewMatrix[1][0] = u.y;
        _viewMatrix[2][0] = u.z;
        _viewMatrix[0][1] = v.x;
        _viewMatrix[1][1] = v.y;
        _viewMatrix[2][1] = v.z;
        _viewMatrix[0][2] = w.x;
        _viewMatrix[1][2] = w.y;
        _viewMatrix[2][2] = w.z;
        _viewMatrix[3][0] = -glm::dot(u, position);
        _viewMatrix[3][1] = -glm::dot(v, position);
        _viewMatrix[3][2] = -glm::dot(w, position);
    }

    void CameraComponent::setViewYXZ()
    {
        setViewYXZ(_owner.Transform->translation, _owner.Transform->rotation);
    }

    void KeyboardController::update(float deltaTime)
    {
        moveInPlaneXZ(deltaTime);
    }

    void KeyboardController::moveInPlaneXZ(float deltaTime)
    {
        // rotate

        glm::vec3 rotate{0.f};

        if (glfwGetKey(_window, Keys.lookLeft) == GLFW_PRESS)
            rotate.y -= 1.0f;
        if (glfwGetKey(_window, Keys.lookRight) == GLFW_PRESS)
            rotate.y += 1.0f;
        if (glfwGetKey(_window, Keys.lookUp) == GLFW_PRESS)
            rotate.x += 1.0f;
        if (glfwGetKey(_window, Keys.lookDown) == GLFW_PRESS)
            rotate.x -= 1.0f;

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            _owner.Transform->rotation += _lookSpeed * deltaTime * glm::normalize(rotate);

        // limit rotation to ~80 degrees
        _owner.Transform->rotation.x = glm::clamp(_owner.Transform->rotation.x, -1.5f, 1.5f);
        _owner.Transform->rotation.y = glm::mod(_owner.Transform->rotation.y, glm::two_pi<float>());


        // move

        float yaw = _owner.Transform->rotation.y;
        const glm::vec3 forwardDir{
            glm::sin(yaw),
            0.f,
            glm::cos(yaw)
        };
        const glm::vec3 rightDir{
            forwardDir.z,
            0.f,
            -forwardDir.x
        };
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        glm::vec3 moveDir{0.f};

        if (glfwGetKey(_window, Keys.moveLeft) == GLFW_PRESS)
            moveDir -= rightDir;
        if (glfwGetKey(_window, Keys.moveRight) == GLFW_PRESS)
            moveDir += rightDir;
        if (glfwGetKey(_window, Keys.moveForward) == GLFW_PRESS)
            moveDir += forwardDir;
        if (glfwGetKey(_window, Keys.moveBackward) == GLFW_PRESS)
            moveDir -= forwardDir;
        if (glfwGetKey(_window, Keys.moveUp) == GLFW_PRESS)
            moveDir += upDir;
        if (glfwGetKey(_window, Keys.moveDown) == GLFW_PRESS)
            moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
            _owner.Transform->translation += _moveSpeed * deltaTime * glm::normalize(moveDir);
    }

    MeshComponent::MeshComponent(GameObject& owner, VGE::VgeDevice& device, const char *filename)
        : Component(owner)
    {
        _mesh = VGE::VgeMesh::createModelFromFile(device, filename);
        
    }
}