//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_CUBE_H
#define VULKANTESTING_CUBE_H

#include <vector>
#include "Node.h"
#include <iostream>

class Cube : public Node{
public:
    Cube(){
        Plane p1 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.5f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         glm::vec3(1.0f,0.4f,0.0f));
        Plane p2 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         glm::vec3(1.0f,0.4f,0.0f));
        Plane p3 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.5f)),
                         glm::vec3(1.0f,0.4f,0.0f));
        Plane p4 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-0.5f)) * glm::rotate(glm::mat4(1.0f),glm::radians(180.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         glm::vec3(1.0f,0.4f,0.0f));
        Plane p5 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.0f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f)),
                         glm::vec3(1.0f,0.4f,0.0f));
        Plane p6 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,0.0f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f)),
                         glm::vec3(1.0f,0.4f,0.0f));

        children.push_back(p1);
        children.push_back(p2);
        children.push_back(p3);
        children.push_back(p4);
        children.push_back(p5);
        children.push_back(p6);
    }

    Cube(glm::vec3 colour){
        Plane p1 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.5f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         colour);
        Plane p2 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         colour);
        Plane p3 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.5f)),
                         colour);
        Plane p4 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-0.5f)) * glm::rotate(glm::mat4(1.0f),glm::radians(180.0f),glm::vec3(1.0f,0.0f,0.0f)),
                         colour);
        Plane p5 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.0f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f)),
                         colour);
        Plane p6 = Plane(glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,0.0f,0.0f)) * glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f)),
                         colour);

        children.push_back(p1);
        children.push_back(p2);
        children.push_back(p3);
        children.push_back(p4);
        children.push_back(p5);
        children.push_back(p6);
    }

    std::vector<Vertex> getVert() override{
        std::vector<Vertex> vertices;
        for(auto & face : children){
            for (auto & v : face.getVert()){
                vertices.push_back(v);
            }
        }
        return vertices;
    }

    std::vector<uint32_t> getInd() override{
        std::vector<uint32_t> indices;
        int offset = 0;
        for(auto & face : children){
            for (auto & ind : face.getInd()){
                indices.push_back(ind+offset);
            }
            offset+=face.getVert().size();
        }
        return indices;
    }

    void transform(glm::mat4 M) override{
        for(auto & face : children){
            face.transform(M);
        }
    }

    Node subdivideNode(Node sourceNode) {
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
};


#endif //VULKANTESTING_CUBE_H
