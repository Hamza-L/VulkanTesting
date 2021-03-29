//
// Created by Hamza Lahmimsi on 2021-03-09.
//

#ifndef VULKANTESTING_NODE_H
#define VULKANTESTING_NODE_H

#include "vert.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Node {
protected:
    std::vector<Node> children{};
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 MinvT = glm::mat4(1.0f);
public:
    void addChild(Node& child){
        children.push_back(child);
    }

    virtual void transform(glm::mat4 M){
        Model = M * Model;
        MinvT = glm::transpose(glm::inverse(Model));
        for (Node child : children){
            child.transform(M);
        }
    }

    virtual std::vector<Vertex> getVert() {
        std::vector<Vertex> outVert;
        glm::vec4 tempPos;
        glm::vec4 tempNorm;
        glm::vec4 tempUp;
        for (auto & currV : vertices){
            Vertex v = currV;
            tempPos = Model * glm::vec4(v.position,1.0f);
            tempNorm = glm::normalize(MinvT * glm::vec4(v.norm,0.0f));
            tempUp = glm::normalize(MinvT * glm::vec4(v.up,0.0f));
            v.position = glm::vec3(tempPos.x, tempPos.y, tempPos.z);
            v.norm = glm::vec3(tempNorm.x, tempNorm.y, tempNorm.z);
            v.up = glm::vec3(tempUp.x, tempUp.y, tempUp.z);
            v.colour = v.colour;
            v.texCoord = v.texCoord;
            outVert.push_back(v);
        }
        for (Node child : children){
            for(Vertex currV : child.getVert()){
                Vertex v = currV;
                tempPos = Model * glm::vec4(v.position,1.0f);
                tempNorm = glm::normalize(MinvT * glm::vec4(v.norm,0.0f));
                tempUp = glm::normalize(MinvT * glm::vec4(v.up,0.0f));
                v.position = glm::vec3(tempPos.x, tempPos.y, tempPos.z);
                v.norm = glm::vec3(tempNorm.x, tempNorm.y, tempNorm.z);
                v.up = glm::vec3(tempUp.x, tempUp.y, tempUp.z);
                v.colour = v.colour;
                v.texCoord = v.texCoord;
                outVert.push_back(v);
            }
        }
        return outVert;
    }

    virtual std::vector<uint32_t> getInd() {
        std::vector<uint32_t> outInd;
        int offset = 0;
        for (auto & ind : indices){
            outInd.push_back(ind);
        }
        offset = vertices.size();
        for(Node child : children){
            for (auto & ind : child.getInd()){
                outInd.push_back(ind+offset);
            }
            offset+=child.getVert().size();
        }
        return outInd;
    }

};

#endif //VULKANTESTING_NODE_H
