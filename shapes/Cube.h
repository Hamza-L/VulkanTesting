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

        /*
        v0.colour = {1.0f,0.0f,0.0f};
        v1.colour = {0.0f,1.0f,0.0f};
        v2.colour = {0.0f,0.0f,1.0f};
        v3.colour = {1.0f,1.0f,0.0f};
        v4.colour = {0.0f,1.0f,1.0f};
        v5.colour = {1.0f,0.0f,1.0f};
        v6.colour = {1.0f,1.0f,1.0f};
        v7.colour = {1.0f,0.0f,0.0f};
         */

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v4);
        vertices.push_back(v5);
        vertices.push_back(v6);
        vertices.push_back(v7);

        indices = {0,1,3,2,7,6,4,5,0,1,2,5,6,4,7,0,3};
    }

    std::vector<Vertex> getVert(){
        return vertices;
    }

    std::vector<uint16_t> getInd(){
        return indices;
    }
};


#endif //VULKANTESTING_CUBE_H
