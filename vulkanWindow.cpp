//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#include "vulkanWindow.h"
#include <stdexcept>

namespace hva {

    VulkanWindow::VulkanWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }

    VulkanWindow::~VulkanWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VulkanWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    }

    void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if(glfwCreateWindowSurface(instance, window, nullptr, surface)!= VK_SUCCESS){
            throw std::runtime_error("failed to create window surface");
        }
    }

    void VulkanWindow::frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto vulkanWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
        vulkanWindow->frameBufferResized = true;
        vulkanWindow->width = width;
        vulkanWindow->height = height;
    }
}