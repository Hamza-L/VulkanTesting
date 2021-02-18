//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_PLANE_H
#define VULKANTESTING_PLANE_H

class Plane {
private:
    int size;
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
public:
    Plane(){
        Vertex v0,v1,v2,v3;
        v0.pos = {0.5f, -0.5f, 0.0f};
        v1.pos = {-0.5f, -0.5f, 0.0f};
        v2.pos = {-0.5f, 0.5f, 0.0f};
        v3.pos = {0.5f, 0.5f, 0.0f};

        v0.colour = {1.0f, 1.0f, 1.0f};
        v1.colour = {0.0f, 1.0f,0.0f};
        v2.colour = {0.0f, 0.0f, 1.0f};
        v3.colour = {1.0f, 0.0f, 0.0f};

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
};

#endif //VULKANTESTING_PLANE_H
