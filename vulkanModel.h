//
// Created by Hamza Lahmimsi on 2021-02-21.
//

/**
 * Allows us to render objects from files
 */

#ifndef VULKANTESTING_VULKANMODEL_H
#define VULKANTESTING_VULKANMODEL_H

#include "vulkanDevice.h"
#include "shapes/vert.h"

//glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

//std
#include <vector>

namespace hva {

    struct PushObject{
        glm::mat4 M = glm::mat4(1.0f);
        glm::mat4 MinvT = glm::mat4(1.0f);
    };

    class VulkanModel{
    public:
        VulkanModel(VulkanDevice &device, const std::vector<Vertex> &vertices, const std::vector<uint32_t>& indices, VkQueue transferQueue, VkCommandPool transferCommandPool, int newTexID);
        ~VulkanModel();

        VulkanModel(const VulkanModel&) = delete;
        VulkanModel &operator=(const VulkanModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        void bindIndexed(VkCommandBuffer commandBuffer);
        void drawIndexed(VkCommandBuffer commandBuffer);

        PushObject* getModel(){return &pobj;};
        void setModel(glm::mat4 M){
            pobj.M = M;
            pobj.MinvT = glm::transpose(glm::inverse(M));
        };

        int getTexID(){return texID;};
        void setTexID(int newTexID){texID = newTexID;};

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices, VkQueue transferQueue, VkCommandPool transferCommandPool);
        void createindexBuffers(const std::vector<uint32_t> &indices, VkQueue transferQueue, VkCommandPool transferCommandPool);

        int texID;


        VulkanDevice &device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        PushObject pobj;
    };
}

#endif //VULKANTESTING_VULKANMODEL_H
