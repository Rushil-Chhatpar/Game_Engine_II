#pragma once

#include "VGE_mesh.hpp"

#include <memory>

namespace VGE
{

    struct Transform2dComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation{0.0f};

        glm::mat2 mat2() 
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);

            /*
            this is the rotation matrix in conventional math
            | c -s |
            | s  c |

            However, in Vulkan, the Y axis is flipped.
            So, I have used the following matrix instead
            | c  s |
            | -s c |
            */
            glm::mat2 rotationMatrix{
                {c, -s},
                {s, c}
            };

            glm::mat2 scaleMatrix{
                {scale.x, 0.0f},
                {0.0f, scale.y}
            };
            return rotationMatrix * scaleMatrix;
        }
    };


    class VgeGameObject
    {
    public:
        using id_t = unsigned int;
        
        // Factory method to create a new game object
        static VgeGameObject createGameObject()
        {
            static id_t currentId = 0;
            return VgeGameObject(currentId++);
        }
        
        // Get the ID of the game object
        id_t getId() const { return _id; }
        
        Transform2dComponent Transform2d{};

        VgeGameObject(const VgeGameObject&) = delete;
        VgeGameObject& operator=(const VgeGameObject&) = delete;

        VgeGameObject(VgeGameObject&&) = default;
        VgeGameObject& operator=(VgeGameObject&&) = default;

        // setters and getters
        void setMesh(std::shared_ptr<VgeMesh> mesh) { _mesh = std::move(mesh); }
        std::shared_ptr<VgeMesh> getMesh() const { return _mesh; }
        void setColor(const glm::vec3& color) { this->color = color; }
        const glm::vec3& getColor() const { return color; }
        
        private:
        VgeGameObject(id_t id) : _id(id) {}

        std::shared_ptr<VgeMesh> _mesh;
        glm::vec3 color{};
        
        id_t _id;

    };
}