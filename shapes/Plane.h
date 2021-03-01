//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_PLANE_H
#define VULKANTESTING_PLANE_H

#include "Shape.h"
#include "glm/glm.hpp"

class Plane : public Shape{
private:
public:
    Plane(){
        Vertex v0,v1,v2,v3;
        v0.pos = {0.5f, -0.5f, 0.0f};
        v1.pos = {-0.5f, -0.5f, 0.0f};
        v2.pos = {-0.5f, 0.5f, 0.0f};
        v3.pos = {0.5f, 0.5f, 0.0f};

        v0.colour = {0.1f, 0.1f, 0.1f};
        v1.colour = {0.1f, 0.1f, 0.1f};
        v2.colour = {0.1f, 0.1f, 0.1f};
        v3.colour = {0.1f, 0.1f, 0.1f};

        v0.norm = {0.0f, 0.0f, 1.0f};
        v1.norm = {0.0f, 0.0f, 1.0f};
        v2.norm = {0.0f, 0.0f, 1.0f};
        v3.norm = {0.0f, 0.0f, 1.0f};

        indices = {1, 0, 2, 3};

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }

    Plane(glm::mat4 M){
        Vertex v0,v1,v2,v3;
        v0.pos = glm:: mat3(M) * glm::vec3(0.5f, -0.5f, 0.0f);
        v1.pos = glm:: mat3(M) * glm::vec3(-0.5f, -0.5f, 0.0f);
        v2.pos = glm:: mat3(M) * glm::vec3(-0.5f, 0.5f, 0.0f);
        v3.pos = glm:: mat3(M) * glm::vec3(0.5f, 0.5f, 0.0f);

        v0.colour = {0.1f, 0.1f, 0.1f};
        v1.colour = {0.1f, 0.1f, 0.1f};
        v2.colour = {0.1f, 0.1f, 0.1f};
        v3.colour = {0.1f, 0.1f, 0.1f};

        v0.norm = glm:: mat3(glm::transpose(glm::inverse(M))) * glm::vec3(0.0f, 0.0f, 1.0f);
        v1.norm = glm:: mat3(glm::transpose(glm::inverse(M))) * glm::vec3(0.0f, 0.0f, 1.0f);
        v2.norm = glm:: mat3(glm::transpose(glm::inverse(M))) * glm::vec3(0.0f, 0.0f, 1.0f);
        v3.norm = glm:: mat3(glm::transpose(glm::inverse(M))) * glm::vec3(0.0f, 0.0f, 1.0f);

        indices = {1, 0, 2, 3};

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }

    std::vector<Vertex> getVert(){
        return vertices;
    }

    std::vector<uint16_t> getInd(){
        return indices;
    }

    void transform(glm::mat4 M){
        glm::mat4 MinvT = glm::transpose(glm::inverse(M));
        for (Vertex vert : vertices){
            vert.pos = vert.pos * glm::mat3(M);
            vert.norm = vert.norm * glm::mat3(MinvT);
            vertices.push_back(vert);
        }
    }
};

#endif //VULKANTESTING_PLANE_H
