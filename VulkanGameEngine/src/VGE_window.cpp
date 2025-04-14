#include "VGE_window.hpp"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

namespace VGE
{
    VgeWindow::VgeWindow(int w, int h, std::string name)
        : _width(w)
        , _height(h)
        , _windowName(name)
    {
        initWindow();
    }

    VgeWindow::~VgeWindow()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void VgeWindow::initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(_window, this);
        glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
        
        if (!_window)
        {
            throw std::runtime_error("failed to create window!");
        }
        
        // Set the window icon
        GLFWimage images[1];
        images[0].pixels = stbi_load("../assets/pp_Beach_icon.png", &images[0].width, &images[0].height, 0, 4);
        if (images[0].pixels)
        {
            glfwSetWindowIcon(_window, 1, images);
            stbi_image_free(images[0].pixels);
        }
        else
        {
            std::runtime_error ("Failed to load icon image!");
        }
    }

    void VgeWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto appWindow = reinterpret_cast<VgeWindow *>(glfwGetWindowUserPointer(window));
        appWindow->_framebufferResized = true;
        appWindow->_width = width;
        appWindow->_height = height;
    }

    void VgeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

}