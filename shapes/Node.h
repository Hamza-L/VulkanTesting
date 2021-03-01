//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_NODE_H
#define VULKANTESTING_NODE_H

#include "Shape.h"
#include "vector"
#include "../vertStruct.h"

class Node {
private:
    std::vector<Vertex> vertices{};
    std::vector<uint16_t> indices{};
    std::vector<Shape> shapes{};
public:
    Node()= default;

    std::vector<Vertex> getVert(){
        for (Shape shape : shapes){
            for (Vertex vert : shape.getVert()){
                vertices.push_back(vert);
            }
        }
        return vertices;
    }

    std::vector<uint16_t> getInd(){
        for (Shape shape : shapes){
            for (uint16_t index : shape.getInd()){
                indices.push_back(index);
            }
        }
        return indices;
    }

    void addShape(Shape inShape){
        shapes.push_back(inShape);
        for(Vertex vert : inShape.getVert()){
            vertices.push_back(vert);
        }
        for(uint16_t ind : inShape.getInd()){
            indices.push_back(ind);
        }
    }
};

#endif //VULKANTESTING_NODE_H
