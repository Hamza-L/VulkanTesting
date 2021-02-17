//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_CUBE_H
#define VULKANTESTING_CUBE_H

#include <vector>
#include "../vertStruct.h"

class Cube {
private:
    int size;
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
public:
    Cube(){
        Vertex v0,v1,v2,v3,v4,v5,v6,v7;
        v0.pos = {-0.5f, -0.5f, 0.5f};
        v1.pos = {0.5f, -0.5f, 0.5f};
        v2.pos = {0.5f, 0.5f, 0.5f};
        v3.pos = {-0.5f, 0.5f, 0.5f};
        v4.pos = {-0.5f, -0.5f, -0.5f};
        v5.pos = {0.5f, -0.5f, -0.5f};
        v6.pos = {0.5f, 0.5f, -0.5f};
        v7.pos = {-0.5f, 0.5f, -0.5f};

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v4);
        vertices.push_back(v5);
        vertices.push_back(v6);
        vertices.push_back(v7);
    }

    std::vector<Vertex> getVert(){
        return vertices;
    }
};


#endif //VULKANTESTING_CUBE_H
