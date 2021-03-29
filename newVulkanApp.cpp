//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#include "newVulkanApp.h"

#include "glm/glm.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <array>
#include <iostream>
#include <mm_malloc.h>
#include <cstdlib>

const int MAX_OBJECTS = 4;
bool UP_PRESS = false;
bool DOWN = false;
bool RIGHT = false;
bool LEFT = false;
bool SHIFT = false;
bool COM = false;

namespace hva{

    NewVulkanApp::NewVulkanApp() {
        createDescriptorSetLayout();
        createPushConstantRange();
        createPipelineLayout();
        recreateSwapchain();
        //firstTex = createTextureImage("test.jpg");
        //allocateDynamicBufferTransferSpace(); only for Dynamic Buffer Uniform Objects
        createTextureSampler();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        loadModels();
    }

    NewVulkanApp::~NewVulkanApp() {
        //_mm_free(modelTransferSpace); //for dynamic Buffer Object

        vkDestroySampler(device.device(), texSampler, nullptr);

        vkDestroyDescriptorPool(device.device(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);

        vkDestroyDescriptorPool(device.device(), samplerDescriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device.device(), samplerSetLayout, nullptr);
        for(size_t i=0; i<vulkanSwapChain->imageCount();i++){
            vkDestroyBuffer(device.device(),vpUniformBuffer[i], nullptr);
            vkFreeMemory(device.device(),vpUniformBufferMemory[i], nullptr);
            //vkDestroyBuffer(device.device(),mDynUniformBuffer[i], nullptr);
            //vkFreeMemory(device.device(),mDynUniformBufferMemory[i], nullptr);
        }

        for (size_t i = 0; i < textureImages.size(); i++){
            vkDestroyImageView(device.device(),textureImageViews[i],nullptr);
            vkDestroyImage(device.device(),textureImages[i],nullptr);
            vkFreeMemory(device.device(), textureImageMemory[i], nullptr);
        }

        for (size_t i = 0; i < textureNormImages.size(); i++){
            vkDestroyImageView(device.device(),textureNormImageViews[i],nullptr);
            vkDestroyImage(device.device(),textureNormImages[i],nullptr);
            vkFreeMemory(device.device(), textureNormImageMemory[i], nullptr);
        }

        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void NewVulkanApp::run(std::string object) {

        while (!vulkanWindow.shouldClose()){
            glfwPollEvents();
            glfwSetKeyCallback(vulkanWindow.getWindow(),key_callback);
            updateModels();
            drawFrame();
        }
        vkDeviceWaitIdle(device.device()); //wait until all GPU operations have been completed
    }

    void NewVulkanApp::loadModels() {

        //setting up the UboVP matrices
        uboVP.P = glm::perspective(glm::radians(25.0f), (float) vulkanSwapChain->width()/(float)vulkanSwapChain->height(),
                                 0.1f, 100.0f);
        uboVP.V = glm::lookAt(glm::vec3(0.0f,0.0f,4.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
        uboVP.lightPos = glm::vec4(-2.0f, 3.0f, 4.0f, 1.0f);
        //std::cout<<uboVP.M[0][0]<<","<<uboVP.M[1][1]<<","<<uboVP.M[2][2]<<","<<uboVP.M[3][3]<<std::endl;

        Node test;
        Cube rect1 = Cube(glm::vec3(1.0f,0.5f,0.0f));
        Cube cube1;
        Icosahedron sphere(10,glm::vec3(1.0f,0.5f,0.0f));
        Plane p1;

        test.addChild(rect1);
        for (int i=0; i<2; i++){
            //test = subdivideNode(test);
        }

        std::string var;

        //glm::mat4 M = glm::translate(glm::rotate(glm::mat4(1.0f),glm::radians(45.0f),glm::vec3(1.0f,1.0f,0.0f)),glm::vec3(1.0f,0.0f,-0.0f));
        //M = glm::scale(M,glm::vec3(0.3f));
        //std::cout<<"the number of vertices is: "<<sphere.getVert().size()<<std::endl;
        //std::cout<<"the number of indices is: "<<sphere.getInd().size()<<std::endl;

        modelList.push_back(std::move(std::make_unique<VulkanModel>(device, cube1.getVert(), cube1.getInd() , device.graphicsQueue(), device.getCommandPool(), createTexture("tiles.jpg","tilesUV.jpg"))));
        modelList.push_back(std::move(std::make_unique<VulkanModel>(device, sphere.getVert(), sphere.getInd() , device.graphicsQueue(), device.getCommandPool(), createTexture("test.jpg",""))));

        //cube1.transform(glm::mat4(1.2f));
        //modelList.push_back(std::move(std::make_unique<VulkanModel>(device, cube1.getVert(), cube1.getInd() , device.graphicsQueue(), device.getCommandPool())));
        //modelList[1]->setModel(glm::mat4(1.2f));
        //modelList.push c _back(std::move(std::make_unique<VulkanModel>(device, scene.getVert(), scene.getInd()  , device.graphicsQueue(), device.getCommandPool())));

    }

    Node NewVulkanApp::subdivideNode(Node sourceNode) {
        Node node1, node2, node3, node4, outNode, tempNode;
        node1 = sourceNode;
        node2 = sourceNode;
        node3 = sourceNode;
        node4 = sourceNode;

        glm::mat4 M = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,0.0f));
        node1.transform(M);
        M = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::translate(glm::mat4(1.0f),glm::vec3(-1.0f,1.0f,0.0f));
        node2.transform(M);
        M = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,-1.0f,0.0f));
        node3.transform(M);
        M = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::translate(glm::mat4(1.0f),glm::vec3(-1.0f,-1.0f,0.0f));
        node4.transform(M);

        tempNode.addChild(node1);
        tempNode.addChild(node2);
        tempNode.addChild(node3);
        tempNode.addChild(node4);

        outNode.addChild(tempNode);

        M =  glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f));
        tempNode.transform(M);
        outNode.addChild(tempNode);

        return outNode;
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

        std::array<VkDescriptorSetLayout,2> descriptorSetLayouts = {descriptorSetLayout, samplerSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    void NewVulkanApp::createDescriptorSetLayout() {

        //uniform value descriptor set layout
        VkDescriptorSetLayoutBinding uboVPLayoutBinding = {};
        uboVPLayoutBinding.binding = 0; //binding location in shader
        uboVPLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //type of descriptor (could be: uniform, dynamic uniform, texture ...)
        uboVPLayoutBinding.descriptorCount = 1; //number of objects/descriptor we are binding.
        uboVPLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //shader stage to bind to.
        uboVPLayoutBinding.pImmutableSamplers = nullptr; //sample data can't be changed. we are not using any texture so we are leaving it null. only the sampler becomes immutable, not the image.

        /*
        VkDescriptorSetLayoutBinding modelLayoutBinding = {}; //for the dynamic model uniform buffer.
        modelLayoutBinding.binding = 1;
        modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        modelLayoutBinding.descriptorCount = 1;
        modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        modelLayoutBinding.pImmutableSamplers = nullptr;*/

        std::vector<VkDescriptorSetLayoutBinding> layoutBindings = {uboVPLayoutBinding};//, modelLayoutBinding};

        VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());  //number of binding infos
        layoutCreateInfo.pBindings = layoutBindings.data(); //pointer to binding info;

        if( vkCreateDescriptorSetLayout(device.device(), &layoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS){
            throw std::runtime_error("Failed to create descriptor set layout!");
        }

        //create texture sampler descriptor set layout;
        //texture binding info
        VkDescriptorSetLayoutBinding samplerLayoutBinding1 = {};
        samplerLayoutBinding1.binding = 0; //from set 1
        samplerLayoutBinding1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding1.descriptorCount = 1;
        samplerLayoutBinding1.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerLayoutBinding1.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding samplerLayoutBinding2 = {};
        samplerLayoutBinding2.binding = 1; //from set 1
        samplerLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding2.descriptorCount = 1;
        samplerLayoutBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerLayoutBinding2.pImmutableSamplers = nullptr;

        std::array<VkDescriptorSetLayoutBinding, 2> samplerlayoutBindings = {samplerLayoutBinding1, samplerLayoutBinding2};

        //create a descriptor set layout with given bindings for texture
        VkDescriptorSetLayoutCreateInfo texLayoutCreateInfo = {};
        texLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        texLayoutCreateInfo.bindingCount = 2;
        texLayoutCreateInfo.pBindings = samplerlayoutBindings.data();

        if( vkCreateDescriptorSetLayout(device.device(), &texLayoutCreateInfo, nullptr, &samplerSetLayout) != VK_SUCCESS){
            throw std::runtime_error("Failed to create sampler descriptor set layout!");
        }
    }

    void NewVulkanApp::createPipeline(int flag) {
        pipelineConfig = VulkanPipeline::defaultPipelineConfigInfo(vulkanSwapChain->width(), vulkanSwapChain->height());
        if (flag==1) {
            pipelineConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
            pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        }
        if (flag==2) {
            pipelineConfig.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            pipelineConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
        }
        pipelineConfig.renderPass = vulkanSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        vulkanPipelines.push_back(std::move(std::make_unique<VulkanPipeline>(device,
                                                          "/Users/hamzalah/Documents/gitProjects/VulkanTest/VulkanTesting/VulkanTesting/shaders/vert.spv",
                                                          "/Users/hamzalah/Documents/gitProjects/VulkanTest/VulkanTesting/VulkanTesting/shaders/frag.spv",
                                                          pipelineConfig)));
    }

    void NewVulkanApp::createUniformBuffers() {

        VkDeviceSize vpBufferSize = sizeof(UboVP); //size of all three variable - will offset to access it;
        //VkDeviceSize mDynBufferSize = modelUniformAlignment * MAX_OBJECTS; //size of all three variable - will offset to access it;


        vpUniformBuffer.resize(vulkanSwapChain->imageCount());
        vpUniformBufferMemory.resize(vulkanSwapChain->imageCount());

        //mDynUniformBuffer.resize(vulkanSwapChain.imageCount());
        //mDynUniformBufferMemory.resize(vulkanSwapChain.imageCount());

        for(size_t i=0; i<vulkanSwapChain->imageCount(); i++){
            device.createBuffer(vpBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                vpUniformBuffer[i], vpUniformBufferMemory[i]);

            //device.createBuffer(mDynBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                //mDynUniformBuffer[i], mDynUniformBufferMemory[i]);
        }
    }

    void NewVulkanApp::createDescriptorPool() {

        //create uniform descriptor pool
        VkDescriptorPoolSize samplerPoolSize = {};
        samplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerPoolSize.descriptorCount = MAX_OBJECTS; //we are assuming each object will have one texture.

        //how many descriptors, not descriptor sets.
        VkDescriptorPoolSize vpPoolSize = {};
        vpPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vpPoolSize.descriptorCount = static_cast<uint32_t>(vpUniformBuffer.size());

        /*//dynamic pool size
        VkDescriptorPoolSize mDynPoolSize = {};
        mDynPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        mDynPoolSize.descriptorCount = static_cast<uint32_t>(mDynUniformBuffer.size());*/

        std::vector<VkDescriptorPoolSize> descriptorPoolSizes = {vpPoolSize}; //, mDynPoolSize}; (for dynamic buffer objects

        VkDescriptorPoolCreateInfo poolCreateInfo = {}; //data to create descriptor pool
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.maxSets = static_cast<uint32_t>(vulkanSwapChain->imageCount());
        poolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size()); //number of poolSize structs we are passing in.
        poolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

        if(vkCreateDescriptorPool(device.device(), &poolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS){
            throw std::runtime_error("failed to create a descriptor pool");
        }

        //create sampler descriptor pool
        //texture sampler pool
        VkDescriptorPoolCreateInfo samplerPoolCreateInfo = {};
        samplerPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        samplerPoolCreateInfo.maxSets = MAX_OBJECTS;
        samplerPoolCreateInfo.poolSizeCount = 1;
        samplerPoolCreateInfo.pPoolSizes = &samplerPoolSize;

        if(vkCreateDescriptorPool(device.device(), &samplerPoolCreateInfo, nullptr, &samplerDescriptorPool) != VK_SUCCESS){
            throw std::runtime_error("failed to create a sampler descriptor pool");
        }
    }

    void NewVulkanApp::createDescriptorSets() {
        descriptorSets.resize(vulkanSwapChain->imageCount()); //resize the descriptor set so there is one for every buffer

        std::vector<VkDescriptorSetLayout> setLayouts(vulkanSwapChain->imageCount(), descriptorSetLayout);

        VkDescriptorSetAllocateInfo setAllocInfo = {}; //descriptor set allocation info
        setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        setAllocInfo.descriptorPool = descriptorPool;
        setAllocInfo.descriptorSetCount = static_cast<uint32_t>(vulkanSwapChain->imageCount());
        setAllocInfo.pSetLayouts = setLayouts.data(); //one to one relationship between the descriptorsetLayout and sets. layout to allocate sets.

        if (vkAllocateDescriptorSets(device.device(), &setAllocInfo, descriptorSets.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate descriptor sets");
        }

        for(size_t i=0; i<vulkanSwapChain->imageCount(); i++){
            VkDescriptorBufferInfo uboVPBufferInfo = {};
            uboVPBufferInfo.buffer = vpUniformBuffer[i];
            uboVPBufferInfo.offset = 0; //if we only want to bind half of the descriptor for example. we want ot start from zero to bind everything
            uboVPBufferInfo.range = sizeof(UboVP); //size of data chunk;

            VkWriteDescriptorSet uboVPSetWrite = {}; //data about connection between buffer and binding. the descriptor set will be bound to the buffer using the following info.
            uboVPSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            uboVPSetWrite.dstSet = descriptorSets[i]; // destination set
            uboVPSetWrite.dstBinding = 0; //which binding in our descriptor set are we updating.
            uboVPSetWrite.dstArrayElement = 0; //index in array to update.
            uboVPSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboVPSetWrite.descriptorCount = 1; //amount to update
            uboVPSetWrite.pBufferInfo = &uboVPBufferInfo; //info about buffer info.

            /*//dynamic buffer binding info
            VkDescriptorBufferInfo mDynBufferInfo = {};
            mDynBufferInfo.buffer = mDynUniformBuffer[i];
            mDynBufferInfo.offset = 0;
            mDynBufferInfo.range = modelUniformAlignment;

            VkWriteDescriptorSet mDynSetWrite = {};
            mDynSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            mDynSetWrite.dstSet = descriptorSets[i];
            mDynSetWrite.dstBinding = 1;
            mDynSetWrite.dstArrayElement = 0;
            mDynSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            mDynSetWrite.descriptorCount = 1;
            mDynSetWrite.pBufferInfo = &mDynBufferInfo; */

            std::vector<VkWriteDescriptorSet> setWrites = {uboVPSetWrite}; //, mDynSetWrite};

            //update the descriptor set with the new buffer/binding info
            vkUpdateDescriptorSets(device.device(), static_cast<uint32_t>(setWrites.size()), setWrites.data(), 0, nullptr);
        }
    }

    void NewVulkanApp::createCommandBuffers() {
        commandBuffers.resize(vulkanSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //submitted directly to queue. can't be called by other command buffers.
        allocateInfo.commandPool = device.getCommandPool();
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers");
        }

    }

    void NewVulkanApp::recordCommand(uint32_t imageIndex){

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        //beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //buffer can be re-submitted when it has already been submitted on the queue and is awaiting for execution.

        //info on how to begin the render pass. only used for graphical application
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkanSwapChain->getRenderPass();
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = vulkanSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.005f, 0.005f, 0.005f, 1.0f}; //clear value for attachment 0 (colour)
        clearValues[1].depthStencil = {1.0f, 0}; //clear value for attachment 1 (depth)
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        renderPassInfo.framebuffer = vulkanSwapChain->getFrameBuffer(imageIndex);

        //start recording command to command buffer!
        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording");
        }

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE); //starts by clearing the image with the clearValue

        //vkCmdSetViewport(commandBuffers[i],0,1,&pipelineConfig.viewport); //flip the viewport along the y axis.
        for (int p=0; p<1; p++) {
            for (int m = 0; m < 1; m++) {
                //std::cout << p << std::endl;
                vulkanPipelines[m]->bind(commandBuffers[imageIndex]);
                modelList[m]->bind(commandBuffers[imageIndex]);
                modelList[m]->bindIndexed(commandBuffers[imageIndex]);

                //uint32_t dynamicOffset = static_cast<uint32_t>(modelUniformAlignment) * m;
                vkCmdPushConstants(commandBuffers[imageIndex],
                                   pipelineLayout,
                                   VK_SHADER_STAGE_VERTEX_BIT,
                                   0,
                                   sizeof(PushObject),
                                   modelList[m]->getModel());

                std::array<VkDescriptorSet, 2> descriptorSetGroup = {descriptorSets[imageIndex], samplerDescriptorSets[modelList[m]->getTexID()]};

                vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        pipelineLayout, 0, 2, descriptorSetGroup.data(),
                                        0,
                                        nullptr); //we have no dynamic offset yet (only using uniform buffer);
                modelList[m]->drawIndexed(commandBuffers[imageIndex]);

            }
        }

        vkCmdEndRenderPass(commandBuffers[imageIndex]);

        // ends the recording of the command buffer!
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }

    }

    void NewVulkanApp::updateUniformBuffers(uint32_t imageIndex) {
        //copy vp data
        void *data;
        vkMapMemory(device.device(), vpUniformBufferMemory[imageIndex], 0, sizeof(UboVP), 0, &data);
        memcpy(data, &uboVP, sizeof(UboVP));
        vkUnmapMemory(device.device(), vpUniformBufferMemory[imageIndex]);

        /** kept for futur reference. Dynamic Buffer memcpy
        //copy model data
        for (size_t i=0; i<modelList.size(); i++){ //here we don't want to use maxobjects since its useless to use memory for objects that dont exist.
            pushObject* thisModel = (pushObject*)((uint64_t)modelTransferSpace + (i * modelUniformAlignment)); //modeltransferspace has just been altered
            *thisModel = modelList[i]->getUbo();
        }
        //map the list of model data
        vkMapMemory(device.device(), mDynUniformBufferMemory[imageIndex], 0, modelUniformAlignment * modelList.size(), 0, &data); //reuse data since it has been freed.
        memcpy(data, modelTransferSpace, modelUniformAlignment * modelList.size());
        vkUnmapMemory(device.device(), mDynUniformBufferMemory[imageIndex]);
         **/
    }

    void NewVulkanApp::allocateDynamicBufferTransferSpace() {
        //find allignment based on the ubo
        //modelUniformAlignment = (sizeof(pushObject) + device.properties.limits.minUniformBufferOffsetAlignment -1) & ~(device.properties.limits.minUniformBufferOffsetAlignment - 1);

        //alocate a certain amount of memory for objects. we have to decide how many objects can be rendered (MAX)
        //modelTransferSpace = (pushObject*)_mm_malloc(modelUniformAlignment * MAX_OBJECTS, modelUniformAlignment);
    }

    void NewVulkanApp::drawFrame(){
        uint32_t imageIndex;
        auto result = vulkanSwapChain->acquireNextImage(&imageIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapchain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error( "failed to acquire swap chain image");
        }

        recordCommand(imageIndex);
        updateUniformBuffers(imageIndex);

        result = vulkanSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanWindow.wasWindowResized()){
            vulkanWindow.resetWindowResizedFlag();
            recreateSwapchain();
            return;
        }

        if (result != VK_SUCCESS) {
            throw std::runtime_error( "failed to present swap chain image");
        }
    }

    std::vector<Vertex> NewVulkanApp::getNormals(std::vector<Vertex> shape) {
        std::vector<Vertex> normalList;

        for(int i=0; i<shape.size(); i++){
            Vertex v1;
            v1.position = shape[i].position;
            v1.colour = glm::vec3(1.0f,1.0f,1.0f);
            Vertex v2;
            v2.position = shape[i].position + shape[i].norm;
            v2.colour = glm::vec3(1.0f,1.0f,1.0f);
            normalList.push_back(v1);
            normalList.push_back(v2);
        }
        return normalList;
    }

    void NewVulkanApp::updateModels() {
        //float sec = glfwGetTime();
        glm::mat4 Scale = glm::scale(glm::mat4(1.0f),glm::vec3(1.1f));
        double xPos,yPos;

        glfwGetCursorPos(vulkanWindow.getWindow(),&xPos,&yPos);
        xPos/=vulkanSwapChain->getSwapChainExtent().width;
        yPos/=vulkanSwapChain->getSwapChainExtent().height;
        xPos = (xPos*4)-1;
        yPos = -(yPos*4)+1;

        uboVP.lightPos = glm::vec4((float)xPos*10.0f,(float)yPos*10.0f,3.2f,1.0f);

        float increment =1.0f;
        if (COM){
            increment = 0.3f;
        }

        if (SHIFT){
            if (RIGHT) {
                M1 = glm::rotate(glm::mat4(1.0f), glm::radians(increment), glm::vec3(0.0f, 0.0f, 1.0f)) * M1;
            }
            if (LEFT) {
                M1 = glm::rotate(glm::mat4(1.0f), glm::radians(-increment), glm::vec3(0.0f, 0.0f, 1.0f)) * M1;
            }
        } else {
            if (UP_PRESS) {
                M1 = glm::rotate(glm::mat4(1.0f), glm::radians(-increment), glm::vec3(1.0f, 0.0f, 0.0f)) * M1;
            }
            if (DOWN) {
                M1 = glm::rotate(glm::mat4(1.0f), glm::radians(increment), glm::vec3(1.0f, 0.0f, 0.0f)) * M1;
            }
            if (RIGHT) {
                M1 = glm::rotate(glm::mat4(1.0f), glm::radians(increment), glm::vec3(0.0f, 1.0f, 0.0f)) * M1;
            }
            if (LEFT) {
                M1 = glm::rotate(glm::mat4(1.0f), glm::radians(-increment), glm::vec3(0.0f, 1.0f, 0.0f)) * M1;
            }
        }

        modelList[0]->setModel(M1);
        modelList[1]->setModel(Scale);
        //M2 =  M2 * glm::rotate(glm::mat4(1.0f), glm::radians(sec * 90.0f),glm::vec3(0.0f,1.0f,0.0f));
        //modelList[0]->setModel(M2);
        //modelList[1]->setModel(Scale*M2);

    }

    void NewVulkanApp::createPushConstantRange() {
        //define push constand values (no 'create' needed);
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;       //offset into given data
        pushConstantRange.size = sizeof(PushObject);
    }

    void NewVulkanApp::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        {

            if (key == GLFW_KEY_UP && action == GLFW_PRESS){
                UP_PRESS = true;
            } else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
                UP_PRESS = false;
            }

            if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
                DOWN = true;
            } else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
                DOWN = false;
            }

            if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
                RIGHT = true;
            } else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
                RIGHT = false;
            }

            if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
                LEFT = true;
            } else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
                LEFT = false;
            }

            if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
                SHIFT = true;
            } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
                SHIFT = false;
            }

            if (key == GLFW_KEY_LEFT_SUPER && action == GLFW_PRESS){
                COM = true;
            } else if (key == GLFW_KEY_LEFT_SUPER && action == GLFW_RELEASE) {
                COM = false;
            }
        }
    }

    void NewVulkanApp::recreateSwapchain() {
        auto extent = vulkanWindow.getExtent();
        while(extent.width == 0 || extent.height == 0){
            extent = vulkanWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.device());
        vulkanSwapChain = std::make_unique<VulkanSwapChain>(device,extent);
        uboVP.P = glm::perspective(glm::radians(65.0f), (float) vulkanSwapChain->width()/(float)vulkanSwapChain->height(),0.1f, 100.0f);
        vulkanPipelines.clear();
        createPipeline(0);
        createPipeline(1);
        createPipeline(0);
    }

    stbi_uc *NewVulkanApp::loadTextureFile(std::string filename, int * width, int * height, VkDeviceSize* imageSize) {
        //number of channels the image usage
        int channels;

        //load pixel data for image
        std::string fileLoc = "textures/" + filename;
        stbi_uc * image = stbi_load(fileLoc.c_str(), width, height, &channels, STBI_rgb_alpha);

        if(image == NULL){
            throw std::runtime_error("failed to load texture file: " + filename );
        }

        *imageSize = (*width) * (*height) * 4; //4 channels

        return image;
    }

    int NewVulkanApp::createTextureImage(std::string filename, std::string norm) {
        int width,height;
        VkDeviceSize imageSize;
        stbi_uc* imageData = loadTextureFile(filename, &width, &height, &imageSize);

        //create staging buffer to hold image data ready to send to device
        VkBuffer imageStagingBuffer;
        VkDeviceMemory imageStagingBufferMemory;

        device.createBuffer(imageSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ,
                            imageStagingBuffer, imageStagingBufferMemory);

        //copy image data to staging buffer;
        void *data;
        vkMapMemory(device.device(), imageStagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, imageData, static_cast<size_t>(imageSize));
        vkUnmapMemory(device.device(), imageStagingBufferMemory);

        //create Vk_image to hold file texture
        VkImage texImage; //VkImage and VkBuffer are both represented by a chunk of data (VkDeviceMemory)
        VkDeviceMemory texMemory;


        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        device.createImageWithInfo(imageInfo,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texImage, texMemory);

        //transition image to be dst for copy operation
        device.transitionImageLayout(device.graphicsQueue(), device.getCommandPool(), texImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        device.copyBufferToImage(imageStagingBuffer, texImage, width, height, 1);
        //transition image to be shader readable for shader usage
        device.transitionImageLayout(device.graphicsQueue(), device.getCommandPool(), texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        //add texture data to the texture list;
        textureImages.push_back(texImage);
        textureImageMemory.push_back(texMemory);

        vkDestroyBuffer(device.device(),imageStagingBuffer, nullptr);
        vkFreeMemory(device.device(),imageStagingBufferMemory, nullptr);

        //norms
        if(!norm.empty()) {
            std::cout<<"hello"<<std::endl;
            stbi_uc *imageDataNorm = loadTextureFile(norm, &width, &height, &imageSize);

            VkBuffer normImageStagingBuffer;
            VkDeviceMemory normImageStagingBufferMemory;

            device.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                normImageStagingBuffer, normImageStagingBufferMemory);

            vkMapMemory(device.device(), normImageStagingBufferMemory, 0, imageSize, 0, &data);
            memcpy(data, imageDataNorm, static_cast<size_t>(imageSize));
            vkUnmapMemory(device.device(), normImageStagingBufferMemory);

            stbi_image_free(imageDataNorm);

            //free original image data
            stbi_image_free(imageData);

            VkImage texImageNorm; //VkImage and VkBuffer are both represented by a chunk of data (VkDeviceMemory)
            VkDeviceMemory texMemoryNorm;
            imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM; //make sure the textures are in normalized coordinates
            device.createImageWithInfo(imageInfo,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texImageNorm, texMemoryNorm);

            device.transitionImageLayout(device.graphicsQueue(), device.getCommandPool(), texImageNorm, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            device.copyBufferToImage(normImageStagingBuffer, texImageNorm, width, height, 1);

            device.transitionImageLayout(device.graphicsQueue(), device.getCommandPool(), texImageNorm, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            textureNormImages.push_back(texImageNorm);
            textureNormImageMemory.push_back(texMemoryNorm);

            vkDestroyBuffer(device.device(),normImageStagingBuffer, nullptr);
            vkFreeMemory(device.device(),normImageStagingBufferMemory, nullptr);

        }

        return textureImages.size()-1;
    }

    int NewVulkanApp::createTexture(std::string fileName, std::string norm) {

        //create texture and get its index
        int textureImageLoc = createTextureImage(fileName,norm);

        //create image view and add to list
        VkImageView imageView = vulkanSwapChain->createImageView(device.device(),textureImages[textureImageLoc],VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        textureImageViews.push_back(imageView);

        int descriptorLoc;

        if(!norm.empty()) {
            VkImageView normImageView = vulkanSwapChain->createImageView(device.device(),
                                                                         textureNormImages[textureImageLoc],
                                                                         VK_FORMAT_R8G8B8A8_UNORM,
                                                                         VK_IMAGE_ASPECT_COLOR_BIT);
            textureNormImageViews.push_back(normImageView);
            descriptorLoc = createTextureAndNormDescriptor(imageView, normImageView);
        } else {
            //TODO: create texture descriptor set here.
            descriptorLoc = createTextureDescriptor(imageView);
        }

        return descriptorLoc;
    }

    void NewVulkanApp::createTextureSampler() {
        //Sampler creation info
        VkSamplerCreateInfo samplerCreateInfo = {};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR; //how to render when image gets magnified on screen.
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR; //how to render when image gets minified on screen.
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; //if we go outside of the texture, how we render? by wrapping around and repeating. (U = x direction, V = y, W = z);
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; //border beyond tex. only works for BORDER_CLAMP
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE; //means it is normalized (between 0-1) coordinates.
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; //as we move away the rate at which it fades away is linear. we won't really see it happen since we have no implemented it yet
        samplerCreateInfo.mipLodBias = 0.0f; //no offset to how the mipmaps are handled.
        samplerCreateInfo.minLod = 0.0f; //min level of detail of mipmap;
        samplerCreateInfo.maxLod = 0.0f; //max level of detail of mipmap;
        samplerCreateInfo.anisotropyEnable = VK_TRUE; //blends pixels when they are stretched due to different angle of views to the texture.
        samplerCreateInfo.maxAnisotropy = 16;

        if(vkCreateSampler(device.device(), &samplerCreateInfo, nullptr, &texSampler) != VK_SUCCESS){
            throw std::runtime_error("failed to create sampler");
        }
    }

    int NewVulkanApp::createTextureDescriptor(VkImageView texImage) {
        VkDescriptorSet descriptorSet;

        //descriptor set allocation info
        VkDescriptorSetAllocateInfo setAllocateInfo = {};
        setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        setAllocateInfo.descriptorPool = samplerDescriptorPool;
        setAllocateInfo.descriptorSetCount = 1;
        setAllocateInfo.pSetLayouts = &samplerSetLayout;

        //allocate Descriptor Set
        if(vkAllocateDescriptorSets(device.device(),&setAllocateInfo,&descriptorSet) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate texture descriptor set");
        }

        //texture image info
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; //what is the image layout when it is in use.
        imageInfo.imageView = texImage; //image to bind to set
        imageInfo.sampler = texSampler;

        //descriptor right info
        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        //update the new descriptorSet
        vkUpdateDescriptorSets(device.device(), 1, &descriptorWrite, 0, nullptr); //update the descriptor set with the new info

        //add descriptor set to list
        samplerDescriptorSets.push_back(descriptorSet);
        return samplerDescriptorSets.size()-1; //return the location of the descriptor set
    }

    int NewVulkanApp::createTextureAndNormDescriptor(VkImageView texImage, VkImageView normImage) {
        VkDescriptorSet descriptorSet;

        //descriptor set allocation info
        VkDescriptorSetAllocateInfo setAllocateInfo = {};
        setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        setAllocateInfo.descriptorPool = samplerDescriptorPool;
        setAllocateInfo.descriptorSetCount = 1;
        setAllocateInfo.pSetLayouts = &samplerSetLayout;

        //allocate Descriptor Set
        if(vkAllocateDescriptorSets(device.device(),&setAllocateInfo,&descriptorSet) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate texture descriptor set");
        }

        //texture image info
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; //what is the image layout when it is in use.
        imageInfo.imageView = texImage; //image to bind to set
        imageInfo.sampler = texSampler;

        VkDescriptorImageInfo normImageInfo = {};
        normImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; //what is the image layout when it is in use.
        normImageInfo.imageView = normImage; //image to bind to set
        normImageInfo.sampler = texSampler;

        //descriptor right info
        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        VkWriteDescriptorSet normDescriptorWrite = {};
        normDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        normDescriptorWrite.dstSet = descriptorSet;
        normDescriptorWrite.dstBinding = 1;
        normDescriptorWrite.dstArrayElement = 0;
        normDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        normDescriptorWrite.descriptorCount = 1;
        normDescriptorWrite.pImageInfo = &normImageInfo;

        std::array<VkWriteDescriptorSet, 2> descriptorSetWrites = {descriptorWrite, normDescriptorWrite};

        //update the new descriptorSet
        vkUpdateDescriptorSets(device.device(), 2, descriptorSetWrites.data(), 0, nullptr); //update the descriptor set with the new info

        //add descriptor set to list
        samplerDescriptorSets.push_back(descriptorSet);
        return samplerDescriptorSets.size()-1; //return the location of the descriptor set
    }

}
