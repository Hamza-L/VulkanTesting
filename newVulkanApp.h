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
#include "stb_image.h"

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

        void run(std::string object);
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
        void recreateSwapchain();
        void drawFrame();
        void updateModels();
        stbi_uc * loadTextureFile(std::string filename, int* width, int* height, VkDeviceSize* imageSize);
        int createTextureImage(std::string filename, std::string norm); //return id of the texture array
        int createTexture(std::string fileName, std::string norm);
        void createTextureSampler();
        int createTextureDescriptor(VkImageView texImage);
        int createTextureAndNormDescriptor(VkImageView texImage, VkImageView normImage);

        int firstTex;

        std::vector<Vertex> getNormals(std::vector<Vertex> shape);

        VulkanWindow vulkanWindow{WIDTH, HEIGHT, "VulkanApp"};
        VulkanDevice device{vulkanWindow};
        std::unique_ptr<VulkanSwapChain> vulkanSwapChain;

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
        VkDescriptorSetLayout samplerSetLayout;
        VkPushConstantRange pushConstantRange;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        VkDescriptorPool samplerDescriptorPool;
        std::vector<VkDescriptorSet> samplerDescriptorSets; //these ones are not 1:1 with each image but with each textures.

        //size_t modelUniformAlignment;
        //pushObject *modelTransferSpace;

        std::unique_ptr<VulkanModel> vulkanModel;
        std::vector<std::unique_ptr<VulkanModel>> modelList;

        //assets
        std::vector<VkImage> textureImages;
        std::vector<VkDeviceMemory> textureImageMemory;
        std::vector<VkImageView> textureImageViews;
        std::vector<VkImage> textureNormImages;
        std::vector<VkDeviceMemory> textureNormImageMemory;
        std::vector<VkImageView> textureNormImageViews;
        VkSampler texSampler;
    };
}

#endif //VULKANTESTING_NEWVULKANAPP_H
