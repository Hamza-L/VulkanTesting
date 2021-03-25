//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_NEWVULKANAPP_H
#define VULKANTESTING_NEWVULKANAPP_H

#include "vulkanWindow.h"
#include "vulkanPipeline.h"
#include "vulkanDevice.h"
#include "vulkanSwapChain.h"
#include "vulkanModel.h"
#include "shapes/Plane.h"
#include "shapes/Cube.h"
#include "shapes/Icosahedron.h"

//std
#include <memory>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>


namespace hva {

    class NewVulkanApp {
    public:
        static constexpr int WIDTH = 1.5*800;
        static constexpr int HEIGHT = 1.5*600;

        NewVulkanApp();
        ~NewVulkanApp();

        NewVulkanApp(const NewVulkanApp&) = delete;
        NewVulkanApp &operator=(const NewVulkanApp&) = delete;

        void run();
        void rename(std::string name){
            vulkanWindow.rename(name);
        };
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        std::vector<Vertex> subdivide(std::vector<Vertex> triangle);
        Node subdivideNode ( Node sourceNode);
    private:
        struct UboVP{
            glm::mat4 V;
            glm::mat4 P;
            glm::vec4 lightPos;
        }uboVP;
        glm::mat4 M1 = glm::mat4(1.0f);
        glm::mat4 M2 = glm::mat4(1.0f);
        void loadModels();
        void createPipelineLayout();
        void createDescriptorSetLayout();
        void createPushConstantRange();
        void createPipeline(int flag);
        void createCommandBuffers();
        void recordCommand(uint32_t imageIndex);
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void updateUniformBuffers(uint32_t imageIndex);
        void allocateDynamicBufferTransferSpace();
        void drawFrame();
        void updateModels();

        std::vector<Vertex> getNormals(std::vector<Vertex> shape);

        VulkanWindow vulkanWindow{WIDTH, HEIGHT, "VulkanApp"};
        VulkanDevice device{vulkanWindow};
        VulkanSwapChain vulkanSwapChain{device, vulkanWindow.getExtent()};

        PipelineConfigInfo pipelineConfig;

        std::vector<std::unique_ptr<VulkanPipeline>> vulkanPipelines;
        int pipelineIndex = 0;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkBuffer> vpUniformBuffer; //create one for every command buffer (swapchain image) so we are assured we are not modifying the buffer as it is being read.
        std::vector<VkDeviceMemory> vpUniformBufferMemory;

        std::vector<VkBuffer> mDynUniformBuffer;
        std::vector<VkDeviceMemory> mDynUniformBufferMemory;


        VkDescriptorSetLayout descriptorSetLayout;
        VkPushConstantRange pushConstantRange;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        //size_t modelUniformAlignment;
        //pushObject *modelTransferSpace;

        std::unique_ptr<VulkanModel> vulkanModel;
        std::vector<std::unique_ptr<VulkanModel>> modelList;
    };
}

#endif //VULKANTESTING_NEWVULKANAPP_H
