//
// Created by Hamza Lahmimsi on 2021-02-19.
//

#ifndef VULKANTESTING_SHAPE_H
#define VULKANTESTING_SHAPE_H

class Shape {
protected:
    int size{};
    std::vector<Vertex> vertices{};
    std::vector<uint16_t> indices{};
public:
    Shape(){
    }

    virtual std::vector<Vertex> getVert(){
        return vertices;
    }

    virtual std::vector<uint16_t> getInd(){
        return indices;
    }
};

#endif //VULKANTESTING_SHAPE_H
