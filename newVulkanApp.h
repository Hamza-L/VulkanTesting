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

//std
#include <memory>
#include <vector>

namespace hva {

    class NewVulkanApp {
    public:
        static constexpr int WIDTH = 1400;
        static constexpr int HEIGHT = 700;

        NewVulkanApp();
        ~NewVulkanApp();

        NewVulkanApp(const NewVulkanApp&) = delete;
        NewVulkanApp &operator=(const NewVulkanApp&) = delete;

        void run();
        void rename(std::string name){
            VulkanWindow.rename(name);
        };
        std::vector<VulkanModel::Vertex> subdivide(std::vector<VulkanModel::Vertex> triangle);
    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        VulkanWindow VulkanWindow{WIDTH, HEIGHT, "VulkanApp"};
        VulkanDevice device{VulkanWindow};
        VulkanSwapChain vulkanSwapChain{device, VulkanWindow.getExtent()};
        std::unique_ptr<VulkanPipeline> vulkanPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<VulkanModel> vulkanModel;
    };
}

#endif //VULKANTESTING_NEWVULKANAPP_H
