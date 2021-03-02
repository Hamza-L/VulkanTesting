//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_VULKANWINDOW_H
#define VULKANTESTING_VULKANWINDOW_H

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
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;

    };
}

#endif //VULKANTESTING_VULKANWINDOW_H
