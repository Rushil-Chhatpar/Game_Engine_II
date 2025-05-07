#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace VGE
{
    class VgeWindow
    {
    public:
        VgeWindow(int w, int h, std::string name);
        ~VgeWindow();

        // Delete copy constructor and assignment operator
        // to prevent copying of the window object
        // This is important because GLFW windows are not copyable
        // and copying them would lead to undefined behavior
        // and resource leaks
        VgeWindow(const VgeWindow&) = delete;
        VgeWindow& operator=(const VgeWindow&) = delete;

        bool shouldClose() { return glfwWindowShouldClose(_window); }
        bool wasWindowResized() { return _framebufferResized; }
        void resetWindowResizedFlag() { _framebufferResized = false; }

        GLFWwindow* getGLFWwindow() const { return _window; }
        VkExtent2D getExtent()
        {
            return VkExtent2D{ static_cast<uint32_t>(_width), static_cast<uint32_t>(_height) };
        }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        void initWindow();
    
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* _window;
        
        int _width;
        int _height;
        std::string _windowName;
        
        bool _framebufferResized = false;
    };
}