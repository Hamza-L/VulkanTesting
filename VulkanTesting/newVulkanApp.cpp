//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#include "newVulkanApp.h"

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
        std::vector<VulkanModel::Vertex> triangle{
                {{-0.8f,-0.8f,0.0f},{1.0f,0.0f,0.0f}},
                {{0.8f,-0.8f,0.0f},{0.0f,1.0f,0.0f}},
                {{0.0f,0.8f,0.0f},{0.0f,0.0f,1.0f}}
        };

        for (int i=0; i<8; i++){
            triangle = subdivide(triangle);
        }

        vulkanModel = std::make_unique<VulkanModel>(device, triangle);
    }

    std::vector<VulkanModel::Vertex> NewVulkanApp::subdivide(std::vector<VulkanModel::Vertex> triangle){
        uint16_t numTriangle = triangle.size()/3;
        std::vector<VulkanModel::Vertex> subd_triangle;
        std::vector<VulkanModel::Vertex> curr_triangle;

        for (int i=0; i<numTriangle; i++) {
            curr_triangle.push_back(triangle[3*i]);
            curr_triangle.push_back(triangle[3*i+1]);
            curr_triangle.push_back(triangle[3*i+2]);
            VulkanModel::Vertex mid1{}, mid2{}, mid3{};

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
        vulkanPipeline = std::make_unique<VulkanPipeline>(device,"shaders/vert.spv","shaders/frag.spv",pipelineConfig);
    }

    void NewVulkanApp::createCommandBuffers(){
        commandBuffers.resize(vulkanSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool = device.getCommandPool();
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers");
        }

        for (int i=0; i<commandBuffers.size(); i++){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
                throw std::runtime_error("failed to begin recording");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = vulkanSwapChain.getRenderPass();
            renderPassInfo.framebuffer = vulkanSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = vulkanSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vulkanPipeline->bind(commandBuffers[i]);
            vulkanModel->bind(commandBuffers[i]);
            vulkanModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
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
