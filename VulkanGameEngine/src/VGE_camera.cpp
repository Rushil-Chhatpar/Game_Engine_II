#include "VGE_camera.hpp"

namespace VGE
{
    void VgeCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        _projectionMatrix[0][0] = 2.0f / (right - left);
        _projectionMatrix[1][1] = 2.0f / (bottom - top);
        _projectionMatrix[2][2] = 1.0f / (far - near);
        _projectionMatrix[3][0] = -(right + left) / (right - left);
        _projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        _projectionMatrix[3][2] = -near / (far - near);
    }

    void VgeCamera::setPerspectiveProjection(float fovY, float aspect, float near, float far)
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
}