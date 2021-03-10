//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_PRIMITIVE_H
#define VULKANTESTING_PRIMITIVE_H

#include "vert.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Primitive {
protected:
    std::vector<Vertex> vertices{};
    std::vector<uint16_t> indices{};
public:
    Primitive(){
    }

    virtual std::vector<Vertex> getVert(){
        return vertices;
    }

    virtual std::vector<uint16_t> getInd(){
        return indices;
    }

    void transform(glm::mat4 M){
        glm::mat4 MinvT = glm::transpose(glm::inverse(M));
        for (int i=0; i<vertices.size(); i++){
            glm::vec4 temp = M * glm::vec4(vertices[i].position,1.0f);
            vertices[i].position = glm::vec3(temp.x, temp.y, temp.z);
            glm::vec4 tempNorm = MinvT * glm::vec4(vertices[i].norm,0.0f);
            vertices[i].norm = glm::normalize(glm::vec3(tempNorm.x, tempNorm.y, tempNorm.z));
        }
    }
};

#endif //VULKANTESTING_PRIMITIVE_H
