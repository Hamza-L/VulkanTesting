//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_VULKANPIPELINE_H
#define VULKANTESTING_VULKANPIPELINE_H

#include "vulkanDevice.h"

#include <string>
#include <vector>

namespace hva{

    struct PipelineConfigInfo{

    };
    class vulkanPipeline{
    public:
        vulkanPipeline(vulkanDevice &device, const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
        ~vulkanPipeline(){}
        vulkanPipeline(const vulkanPipeline&) = delete;
        void operator=(const vulkanPipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filePath);

        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        vulkanDevice &device;
        VkPipeline raphicsPipeline;
        VkShaderModule vertModule;
        VkShaderModule fragModule;
    };

}

#endif //VULKANTESTING_VULKANPIPELINE_H
