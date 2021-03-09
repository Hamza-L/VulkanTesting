//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#include "newVulkanApp.h"

#include "shapes/Plane.h"
#include "shapes/Cube.h"
#include "glm/glm.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <array>
#include <iostream>

namespace hva{

    NewVulkanApp::NewVulkanApp() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    NewVulkanApp::~NewVulkanApp() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void NewVulkanApp::run() {
        while (!VulkanWindow.shouldClose()){
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(device.device()); //wait until all GPU operations have been completed
    }

    void NewVulkanApp::loadModels() {

        //Plane rectangle1 = Plane(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        Cube rectangle;
        Plane p1;
        glm::mat4 M = glm::rotate(glm::mat4(1.0f),glm::radians(50.0f),glm::vec3(1.0f,1.0f,0.0f));
        rectangle.transform(M);

        /*
        std::vector<Vertex> triangle{
                {{-0.8f,-0.8f,0.0f},{1.0f,0.0f,0.0f}},
                {{0.0f,0.8f,0.0f},{0.0f,1.0f,0.0f}},
                {{0.8f,-0.8f,0.0f},{0.0f,0.0f,1.0f}}
        };

        for (int i=0; i<8; i++){
            triangle = subdivide(triangle);
        }
         */

        vulkanModel = std::make_unique<VulkanModel>(device, rectangle.getVert());
    }

    std::vector<Vertex> NewVulkanApp::subdivide(std::vector<Vertex> triangle){
        uint16_t numTriangle = triangle.size()/3;
        std::vector<Vertex> subd_triangle;
        std::vector<Vertex> curr_triangle;

        for (int i=0; i<numTriangle; i++) {
            curr_triangle.push_back(triangle[3*i]);
            curr_triangle.push_back(triangle[3*i+1]);
            curr_triangle.push_back(triangle[3*i+2]);
            Vertex mid1{}, mid2{}, mid3{};

            mid1.position = curr_triangle[0].position + (curr_triangle[1].position - curr_triangle[0].position) / 2.0f;
            mid2.position = curr_triangle[1].position + (curr_triangle[2].position - curr_triangle[1].position) / 2.0f;
            mid3.position = curr_triangle[2].position + (curr_triangle[0].position - curr_triangle[2].position) / 2.0f;

            mid3.colour = curr_triangle[0].colour + (curr_triangle[1].colour - curr_triangle[0].colour) / 2.0f;
            mid1.colour = curr_triangle[1].colour + (curr_triangle[2].colour - curr_triangle[1].colour) / 2.0f;
            mid2.colour = curr_triangle[2].colour + (curr_triangle[0].colour - curr_triangle[2].colour) / 2.0f;

            subd_triangle.push_back(curr_triangle[0]);
            subd_triangle.push_back(mid1);
            subd_triangle.push_back(mid3);

            subd_triangle.push_back(mid1);
            subd_triangle.push_back(curr_triangle[1]);
            subd_triangle.push_back(mid2);

            subd_triangle.push_back(mid3);
            subd_triangle.push_back(mid2);
            subd_triangle.push_back(curr_triangle[2]);

            curr_triangle.clear();
        }

        return subd_triangle;
    }

    void NewVulkanApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    void NewVulkanApp::createPipeline() {
        auto pipelineConfig = VulkanPipeline::defaultPipelineConfigInfo(vulkanSwapChain.width(), vulkanSwapChain.height());
        pipelineConfig.renderPass = vulkanSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        vulkanPipeline = std::make_unique<VulkanPipeline>(device,
                                                          "/Users/hamzalah/Documents/gitProjects/VulkanTest/VulkanTesting/VulkanTesting/shaders/vert.spv",
                                                          "/Users/hamzalah/Documents/gitProjects/VulkanTest/VulkanTesting/VulkanTesting/shaders/frag.spv",
                                                          pipelineConfig);
    }

    void NewVulkanApp::createCommandBuffers(){
        commandBuffers.resize(vulkanSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //submitted directly to queue. can't be called by other command buffers.
        allocateInfo.commandPool = device.getCommandPool();
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers");
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        //beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //buffer can be re-submitted when it has already been submitted on the queue and is awaiting for execution.

        //info on how to begin the render pass. only used for graphical application
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkanSwapChain.getRenderPass();
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = vulkanSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.005f, 0.005f, 0.005f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        for (int i=0; i<commandBuffers.size(); i++){

            //start recording command to command buffer!
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
                throw std::runtime_error("failed to begin recording");
            }

            renderPassInfo.framebuffer = vulkanSwapChain.getFrameBuffer(i);

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); //starts by clearing the image with the clearValue

                vulkanPipeline->bind(commandBuffers[i]);
                vulkanModel->bind(commandBuffers[i]);
                vulkanModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);

            // ends the recording of the command buffer!
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
                throw std::runtime_error("failed to record command buffer");
            }
        }
    }

    void NewVulkanApp::drawFrame(){
        uint32_t imageIndex;
        auto result = vulkanSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error( "failed to acquire swap chain image");
        }

        result = vulkanSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result != VK_SUCCESS) {
            throw std::runtime_error( "failed to present swap chain image");
        }
    }
}
