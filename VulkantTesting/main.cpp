//
//  main.cpp
//  VulkantTesting
//
//  Created by Hamza Lahmimsi on 2020-12-25.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication {
private:
    GLFWwindow* window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    
    void initWindow(){
        glfwInit();
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //prevent glfw from loading opengl context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //prevents allowing the window to be resized
        
        window = glfwCreateWindow(WIDTH, HEIGHT, "VulkantTesting", nullptr, nullptr);
    }
    
    void initVulkan(){
    
    }
  
    void mainLoop(){
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();
        }
    }
    
    void cleanup(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }
    
    
    
};


int main() {
    
    HelloTriangleApplication app;
    
    try{
        app.run();
    } catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout<<"hello"<<std::endl;
    
    return EXIT_SUCCESS;
}
