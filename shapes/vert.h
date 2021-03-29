//
// Created by Hamza Lahmimsi on 2021-03-08.
//

#ifndef VULKANTESTING_VERT_H
#define VULKANTESTING_VERT_H

//vulkan & glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

//std
#include <vector>

struct Vertex{
    glm::vec3 position{};
    glm::vec3 colour{};
    glm::vec3 norm{};
    glm::vec3 up{};
    glm::vec2 texCoord{}; //UV coord for textures.

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0; //can bind multiple stream of Data. this defines which one.
        bindingDescriptions[0].stride = sizeof(Vertex); //size of individual vertex object. how much data does it need to skip over to get to the next vertex
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //draw one vertex at a time; how to move from vertex to vertex. can make it move to a vertex for the next instance.
        return bindingDescriptions;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(5);
        attributeDescriptions[0].binding = 0; //which binding is the attribute at
        attributeDescriptions[0].location = 0; //location where the attribute is bound (seen in the vertex shader)
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; //format of the attribute
        attributeDescriptions[0].offset = offsetof(Vertex, position); //how much data it occupies.

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, colour);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, norm);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, up);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

#endif //VULKANTESTING_VERT_H
