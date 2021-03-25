//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#include "vulkanPipeline.h"

//std
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace hva{

    VulkanPipeline::VulkanPipeline(VulkanDevice &device,
                                   const std::string &vertFilePath,
                                   const std::string &fragFilePath,
                                   const PipelineConfigInfo &configInfo) : device{device}{
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    VulkanPipeline::~VulkanPipeline() {
        vkDestroyShaderModule(device.device(), vertModule, nullptr);
        vkDestroyShaderModule(device.device(), fragModule, nullptr);
        vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
    }

    std::vector<char> VulkanPipeline::readFile(const std::string &filePath){
        std::ifstream file{filePath,std::ios::ate | std::ios::binary};
        if (!file.is_open()){
            throw std::runtime_error("failed to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void VulkanPipeline::createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath,const PipelineConfigInfo &configInfo){
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        /**
        assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
                "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
        assert(configInfo.renderPass != VK_NULL_HANDLE &&
                "Cannot create graphics pipeline:: no renderPass provided in configInfo");
                **/
        createShaderModule(vertCode, &vertModule);
        createShaderModule(fragCode, &fragModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        auto bindingDescriptions = Vertex::getBindingDescriptions();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions =  attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &configInfo.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &configInfo.scissor;

        /*
       // -- DYNAMIC STATE ENABLE
       std::vector<VkDynamicState> dynamicStateEnables;
       dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT); //dynamic viewport. allows us to resize with VkCmdSetViewport(commandBuffer, 0, 1, &newViewPort)
       dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);  //dynamic scissor . allows us to resize with VkCmdSetScissor(commandBuffer, 0, 1, &newScissor)
       // need to resize everything and destroy/recreate swapchain. All images must be resized.

       VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
       dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
       dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
       dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();
       */

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;                                        // number of shader stages
        pipelineInfo.pStages = shaderStages;                                // list of shader stages
        pipelineInfo.pVertexInputState = &vertexInputInfo;                  // all the fixed function pipeline states
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pDynamicState = nullptr; //not used yet
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = configInfo.pipelineLayout;    // Pipeline Layout the pipeline should use.
        pipelineInfo.renderPass = configInfo.renderPass;    // renderpass description the pipeline is compatible with
        pipelineInfo.subpass = configInfo.subpass;          // subpass of renderpass

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS){
            throw std::runtime_error("failed to create graphics pipeline");
        }
    }

    void VulkanPipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule){
        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std::runtime_error("failed to create shader module");
        }
    }

    void VulkanPipeline::bind(VkCommandBuffer commandBuffer){
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    }

    PipelineConfigInfo VulkanPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){
        PipelineConfigInfo configInfo{};
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = static_cast<float>(height);
        configInfo.viewport.width = static_cast<float>(width);
        configInfo.viewport.height = -static_cast<float>(height);
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;

        configInfo.scissor.offset = {0, 0};
        configInfo.scissor.extent = {width, height};

        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; //clamples fragment beyond far plane unto far plane (need GPU feature enabled)
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; //removes the fragment stage. no image generated
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f; // width of lines.
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT; //culls a face of the triangle
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; //which way is front
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE; //depth bias to avoid "shadow acne"
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        configInfo.colorBlendAttachment.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT; //which colours in our image shall we apply the blending to
        configInfo.colorBlendAttachment.blendEnable = VK_TRUE; //enable blending
        //blending equation: (VK_BLEND_FACTOR_SRC_ALPHA * newColour) VK_BLEND_OP_ADD[+] (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * OldColour);
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;   // src
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;  // 1-src
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // a mathematical operation
        //basically" ( 1 * newAlpha ) + ( 0 * oldAlpha) = newAlpha
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // get rid of the old alpha and keep the new alpha.

        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // we don't want to use the logical operator, we want to use the colorBlendAttachment instead
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        //configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        //configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        //configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        //configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE; //enable checking depth to see if fragment pizel should be rendered.
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE; //can we replace the value with a new depth value. we want to update the depth value so we can add objects.
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS; //how we compare depth value.
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE; //depth bound test.. does the depth value exists between two bounds.
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE; //disabled for now.
        configInfo.depthStencilInfo.front = {};  // Optional used for stencils
        configInfo.depthStencilInfo.back = {};   // Optional used for stencils

        return configInfo;
    }


}