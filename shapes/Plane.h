//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_PLANE_H
#define VULKANTESTING_PLANE_H

#include "Node.h"

class Plane : public Node {
public:
    Plane(){
        Vertex v0,v1,v2,v3;
        v0.position = {0.5f, -0.5f, 0.0f};
        v1.position = {-0.5f, -0.5f, 0.0f};
        v2.position = {-0.5f, 0.5f, 0.0f};
        v3.position = {0.5f, 0.5f, 0.0f};

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
        vertices.push_back(v0);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }

    Plane(glm::mat4 M){
        glm::mat4 MinvT = glm::transpose(glm::inverse(M));
        Vertex v0,v1,v2,v3;
        glm::vec4 t0,t1,t2,t3;
        t0 = M * glm::vec4(0.5f, -0.5f, 0.0f,1.0f);
        t1 = M * glm::vec4(-0.5f, -0.5f, 0.0f,1.0f);
        t2 = M * glm::vec4(-0.5f, 0.5f, 0.0f,1.0f);
        t3 = M * glm::vec4(0.5f, 0.5f, 0.0f,1.0f);

        v0.position = glm::vec3(t0.x,t0.y,t0.z);
        v1.position = glm::vec3(t1.x,t1.y,t1.z);
        v2.position = glm::vec3(t2.x,t2.y,t2.z);
        v3.position = glm::vec3(t3.x,t3.y,t3.z);

        v0.colour = {0.1f, 0.1f, 0.1f};
        v1.colour = {0.1f, 0.1f, 0.1f};
        v2.colour = {0.1f, 0.1f, 0.1f};
        v3.colour = {0.1f, 0.1f, 0.1f};

        t0 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
        t1 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
        t2 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
        t3 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);

        v0.norm = glm::vec3(t0.x,t0.y,t0.z);
        v1.norm = glm::vec3(t1.x,t1.y,t1.z);
        v2.norm = glm::vec3(t2.x,t2.y,t2.z);
        v3.norm = glm::vec3(t3.x,t3.y,t3.z);

        indices = {1, 0, 2, 3};

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v0);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }

    Plane(glm::mat4 M, glm::vec3 colour){
        glm::mat4 MinvT = glm::transpose(glm::inverse(M));
        Vertex v0,v1,v2,v3;
        glm::vec4 t0,t1,t2,t3;
        t0 = M * glm::vec4(0.5f, -0.5f, 0.0f,1.0f);
        t1 = M * glm::vec4(-0.5f, -0.5f, 0.0f,1.0f);
        t2 = M * glm::vec4(-0.5f, 0.5f, 0.0f,1.0f);
        t3 = M * glm::vec4(0.5f, 0.5f, 0.0f,1.0f);

        v0.position = glm::vec3(t0.x,t0.y,t0.z);
        v1.position = glm::vec3(t1.x,t1.y,t1.z);
        v2.position = glm::vec3(t2.x,t2.y,t2.z);
        v3.position = glm::vec3(t3.x,t3.y,t3.z);

        v0.colour = colour;
        v1.colour = colour;
        v2.colour = colour;
        v3.colour = colour;

        t0 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
        t1 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
        t2 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
        t3 = MinvT * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);

        v0.norm = glm::vec3(t0.x,t0.y,t0.z);
        v1.norm = glm::vec3(t1.x,t1.y,t1.z);
        v2.norm = glm::vec3(t2.x,t2.y,t2.z);
        v3.norm = glm::vec3(t3.x,t3.y,t3.z);

        indices = {1, 0, 2, 3};

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v0);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }
};

#endif //VULKANTESTING_PLANE_H
