//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_VULKANWINDOW_H
#define VULKANTESTING_VULKANWINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace hva {

    class vulkanWindow {
    public:
        vulkanWindow(int w, int h, std::string name);
        ~vulkanWindow();

        vulkanWindow(const vulkanWindow &) = delete;
        vulkanWindow &operator=(const vulkanWindow&) = delete;

        bool shouldClose(){ return glfwWindowShouldClose(window);};
        void rename(std::string name){
            windowName = name;
            glfwDestroyWindow(window);
            window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        };

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;

    };
}

#endif //VULKANTESTING_VULKANWINDOW_H
