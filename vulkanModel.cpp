//
// Created by Hamza Lahmimsi on 2021-02-21.
//

#include "vulkanModel.h"

//std
#include <cassert>
#include <cstring>
#include <iostream>

namespace hva {

    VulkanModel::VulkanModel(VulkanDevice &device, const std::vector<Vertex> &vertices, const std::vector<uint32_t>& indices, VkQueue transferQueue, VkCommandPool transferCommandPool, int newTexID) : device{device}{
        createVertexBuffers(vertices, transferQueue, transferCommandPool);
        createindexBuffers(indices, transferQueue, transferCommandPool);
        texID = newTexID;
    }

    VulkanModel::~VulkanModel() {
        vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
        vkDestroyBuffer(device.device(), indexBuffer, nullptr);
        vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
        vkFreeMemory(device.device(), indexBufferMemory, nullptr);
    }

    void VulkanModel::createVertexBuffers(const std::vector<Vertex> &vertices, VkQueue transferQueue, VkCommandPool transferCommandPool) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        device.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            stagingBuffer,
                            stagingBufferMemory);
        void *data; //create pointer to a point in normal memory
        vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data); //
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.device(), stagingBufferMemory);

        //Create Buffer with transfer dst bit to mark as recipient of transfer data (ie GPU)
        //buffer memory to be device local bit meaning it is local to GPU and only accessible by it.
        device.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                            vertexBuffer,
                            vertexBufferMemory);

        //copy staging buffer to vertex buffer in GPU
        //now the vertex buffer is fully optimized for GPU usage. it is no longer accessible from the CPU however
        device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize, transferQueue, transferCommandPool);

        vkDestroyBuffer(device.device(),stagingBuffer,nullptr);
        vkFreeMemory(device.device(), stagingBufferMemory, nullptr);

    }

    void VulkanModel::createindexBuffers(const std::vector<uint32_t> &indices, VkQueue transferQueue,
                                         VkCommandPool transferCommandPool) {
        //get size of buffer for indices
        indexCount = static_cast<uint32_t>(indices.size());
        assert(indexCount >= 3 && "indexCount count must be at least 3");
        VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        device.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            stagingBuffer,
                            stagingBufferMemory);
        void *data; //create pointer to a point in normal memory
        vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data); //
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.device(), stagingBufferMemory);

        //create buffer for indices on GPU local
        device.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                            indexBuffer,
                            indexBufferMemory);

        //copy from staging buffer to GPU access buffer
        device.copyBuffer(stagingBuffer, indexBuffer, bufferSize, transferQueue, transferCommandPool);

        vkDestroyBuffer(device.device(),stagingBuffer,nullptr);
        vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
    }

    void VulkanModel::draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    void VulkanModel::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void VulkanModel::drawIndexed(VkCommandBuffer commandBuffer) { //bind model index buffer
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }

    void VulkanModel::bindIndexed(VkCommandBuffer commandBuffer) { //bind model index buffer
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}
