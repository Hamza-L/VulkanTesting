//
// Created by Hamza Lahmimsi on 2021-02-17.
//

#ifndef VULKANTESTING_TRIANGLE_H
#define VULKANTESTING_TRIANGLE_H

class Triangle {
private:
    int size;
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
public:
    Triangle(){
        Vertex v0,v1,v2;
        v0.pos = {0.0f, -0.5f, 0.0f};
        v1.pos = {0.5f, 0.5f, 0.0f};
        v2.pos = {-0.5f, 0.5f, 0.0f};

        v0.colour = {1.0f, 1.0f, 1.0f};
        v1.colour = {0.0f, 1.0f,0.0f};
        v2.colour = {0.0f, 0.0f, 1.0f};

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
    }

    std::vector<Vertex> getVert(){
        return vertices;
    }
};

#endif //VULKANTESTING_TRIANGLE_H
