//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_VULKANWINDOW_H
#define VULKANTESTING_VULKANWINDOW_H

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace hva {

    class VulkanWindow{
    public:
        VulkanWindow(int w, int h, std::string name);
        ~VulkanWindow();

        VulkanWindow(const VulkanWindow &) = delete;
        VulkanWindow &operator=(const VulkanWindow&) = delete;

        bool shouldClose(){ return glfwWindowShouldClose(window);};
        bool wasWindowResized(){return frameBufferResized;};
        void resetWindowResizedFlag(){frameBufferResized = false;};
        GLFWwindow* getWindow(){return window;}
        void rename(std::string name){
            windowName = name;
            glfwDestroyWindow(window);
            window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        };

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        VkExtent2D getExtent() {
            return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        };

    private:
        static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool frameBufferResized;

        std::string windowName;
        GLFWwindow *window;

    };
}

#endif //VULKANTESTING_VULKANWINDOW_H
