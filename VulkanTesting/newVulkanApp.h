//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_NEWVULKANAPP_H
#define VULKANTESTING_NEWVULKANAPP_H

#include "vulkanWindow.h"
#include "vulkanPipeline.h"
#include "vulkanDevice.h"

namespace hva {

    class newVulkanApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
        void rename(std::string name){
            vulkanWindow.rename(name);
        };
    private:
        vulkanWindow vulkanWindow{WIDTH, HEIGHT, "VulkanApp"};
        vulkanDevice device{vulkanWindow};
        vulkanPipeline vulkanPipeline{device, "shaders/vert.spv","shaders/frag.spv", vulkanPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}

#endif //VULKANTESTING_NEWVULKANAPP_H
