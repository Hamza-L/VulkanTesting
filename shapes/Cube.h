//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_CUBE_H
#define VULKANTESTING_CUBE_H

#include <vector>
#include "Plane.h"
#include <iostream>

class Cube {
private:
    std::vector<Shape> shapes;

public:
    Cube(){
        Plane p1 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.5f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         glm::vec3(1.0f,0.0f,0.0f));
        Plane p2 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         glm::vec3(1.0f,0.0f,0.0f));
        Plane p3 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.5f)),
                         glm::vec3(1.0f,0.0f,0.0f));
        Plane p4 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-0.5f)) * glm::rotate(glm::mat4(1.0f),glm::radians(180.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         glm::vec3(1.0f,0.0f,0.0f));
        Plane p5 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.0f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f)),
                         glm::vec3(1.0f,0.0f,0.0f));
        Plane p6 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,0.0f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f)),
                         glm::vec3(1.0f,0.0f,0.0f));

        shapes.push_back(p1);
        shapes.push_back(p2);
        shapes.push_back(p3);
        shapes.push_back(p4);
        shapes.push_back(p5);
        shapes.push_back(p6);
    }

    std::vector<Vertex> getVert(){
        std::vector<Vertex> vertices;
        for (Shape s : shapes){
            for(Vertex v : s.getVert()){
                vertices.push_back(v);
            }
        }
        return vertices;
    }

    std::vector<uint16_t> getInd(){
        std::vector<uint16_t> indices;
        for (Shape s : shapes){
            for(uint16_t i : s.getInd()){
                indices.push_back(i);
            }
        }
        return indices;
    }

    void transform(glm::mat4 M){
        auto temp = glm::vec4(0.0f);
        for (int i = 0; i<shapes.size(); i++){
            shapes[i].transform(M);
        }
    }
};


#endif //VULKANTESTING_CUBE_H
