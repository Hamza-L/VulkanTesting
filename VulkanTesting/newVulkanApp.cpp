//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#include "newVulkanApp.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace hva{

    void newVulkanApp::run() {
        while (!vulkanWindow.shouldClose()){
            glfwPollEvents();
        }
    }
}
