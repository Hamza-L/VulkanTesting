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
    class VulkanModel{
    public:
        VulkanModel(VulkanDevice &device, const std::vector<Vertex> &vertices);
        ~VulkanModel();

        VulkanModel(const VulkanModel&) = delete;
        VulkanModel &operator=(const VulkanModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        VulkanDevice &device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}

#endif //VULKANTESTING_VULKANMODEL_H
