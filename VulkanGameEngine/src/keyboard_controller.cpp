#include "keyboard_controller.hpp"

#include <limits>

namespace VGE
{

    void KeyboardController::moveInPlaneXZ(GLFWwindow *window, float deltaTime, VgeGameObject &gameObject)
    {

        // rotate

        glm::vec3 rotate{0.f};

        if (glfwGetKey(window, Keys.lookLeft) == GLFW_PRESS)
            rotate.y -= 1.0f;
        if (glfwGetKey(window, Keys.lookRight) == GLFW_PRESS)
            rotate.y += 1.0f;
        if (glfwGetKey(window, Keys.lookUp) == GLFW_PRESS)
            rotate.x += 1.0f;
        if (glfwGetKey(window, Keys.lookDown) == GLFW_PRESS)
            rotate.x -= 1.0f;

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            gameObject.Transform.rotation += LookSpeed * deltaTime * glm::normalize(rotate);

        // limit rotation to ~80 degrees
        gameObject.Transform.rotation.x = glm::clamp(gameObject.Transform.rotation.x, -1.5f, 1.5f);
        gameObject.Transform.rotation.y = glm::mod(gameObject.Transform.rotation.y, glm::two_pi<float>());


        // move

        float yaw = gameObject.Transform.rotation.y;
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

        if (glfwGetKey(window, Keys.moveLeft) == GLFW_PRESS)
            moveDir -= rightDir;
        if (glfwGetKey(window, Keys.moveRight) == GLFW_PRESS)
            moveDir += rightDir;
        if (glfwGetKey(window, Keys.moveForward) == GLFW_PRESS)
            moveDir += forwardDir;
        if (glfwGetKey(window, Keys.moveBackward) == GLFW_PRESS)
            moveDir -= forwardDir;
        if (glfwGetKey(window, Keys.moveUp) == GLFW_PRESS)
            moveDir += upDir;
        if (glfwGetKey(window, Keys.moveDown) == GLFW_PRESS)
            moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
            gameObject.Transform.translation += MoveSpeed * deltaTime * glm::normalize(moveDir);
    }
}