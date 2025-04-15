#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace VGE
{
    class VgeCamera
    {
        public:

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        
        void setPerspectiveProjection(float fovY, float aspect, float near, float far);
        
        const glm::mat4& getProjectionMatrix() const { return _projectionMatrix; }
        
        private:
        glm::mat4 _projectionMatrix{1.0f};
    };
}
//_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
//_projectionMatrix = glm::perspective(glm::radians(fovY), aspect, near, far);