#pragma once

#include "VGE_mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace VGE
{

    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{};

        /*
        Use the property values (translation, scale, rotation) to create a 4x4 transformation matrix
        The order of transformations is:
        1. Scale
        2. Rotate on Z axis
        3. Rotate on X axis
        4. Rotate on Y axis
        5. Translate

        The order of transformations is important because matrix multiplication is not commutative.
        Rotation convention uses tait-bryan angles with axix order Y(1), X(2), Z(3)
        */
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
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
        
        TransformComponent Transform{};

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














            // #pragma region Slow method
            // /*
            // create a 4x4 identity matrix and add translation to it
            
            // | 1 0 0 x |
            // | 0 1 0 y |
            // | 0 0 1 z |
            // | 0 0 0 1 |
            // */
            // glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);

            // // rotation
            // transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
            // transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
            // transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
            
            // // scale
            // transform = glm::scale(transform, scale);
            // return transform;
            // #pragma endregion




                    // 2D ONLY
        // glm::mat2 mat2() 
        // {
        //     const float s = glm::sin(rotation);
        //     const float c = glm::cos(rotation);

        //     /*
        //     this is the rotation matrix in conventional math
        //     | c -s |
        //     | s  c |

        //     However, in Vulkan, the Y axis is flipped.
        //     So, I have used the following matrix instead
        //     | c  s |
        //     | -s c |
        //     */
        //     glm::mat2 rotationMatrix{
        //         {c, -s},
        //         {s, c}
        //     };

        //     glm::mat2 scaleMatrix{
        //         {scale.x, 0.0f},
        //         {0.0f, scale.y}
        //     };
        //     return rotationMatrix * scaleMatrix;
        // }