//
// Created by Hamza Lahmimsi on 2021-03-12.
//

#ifndef VULKANTESTING_ICOSAHEDRON_H
#define VULKANTESTING_ICOSAHEDRON_H

#include "Node.h"

class Icosahedron : public Node{
public:
    Icosahedron() {

        float phi = (1 + glm::sqrt(5.0f)) / 2.0f;
        Vertex v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;

        v0.position = {-1.0f,  phi, 0.0f}; v0.colour = {1.0f,1.0f,0.0f};
        v1.position = { 1.0f,  phi, 0.0f}; v1.colour = {1.0f,1.0f,0.0f};
        v2.position = {-1.0f, -phi, 0.0f}; v2.colour = {1.0f,1.0f,0.0f};
        v3.position = { 1.0f, -phi, 0.0f}; v3.colour = {1.0f,1.0f,0.0f};

        v4.position = {0.0f, -1.0f,  phi}; v4.colour = {1.0f,1.0f,0.0f};
        v5.position = {0.0f,  1.0f,  phi}; v5.colour = {1.0f,1.0f,0.0f};
        v6.position = {0.0f, -1.0f, -phi}; v6.colour = {1.0f,1.0f,0.0f};
        v7.position = {0.0f,  1.0f, -phi}; v7.colour = {1.0f,1.0f,0.0f};

        v8.position =  { phi, 0.0f, -1.0f};  v8.colour = {1.0f,1.0f,0.0f};
        v9.position =  { phi, 0.0f,  1.0f};  v9.colour = {1.0f,1.0f,0.0f};
        v10.position = {-phi, 0.0f, -1.0f}; v10.colour = {1.0f,1.0f,0.0f};
        v11.position = {-phi, 0.0f,  1.0f}; v11.colour = {1.0f,1.0f,0.0f};


        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);

        vertices.push_back(v4);
        vertices.push_back(v5);
        vertices.push_back(v6);
        vertices.push_back(v7);

        vertices.push_back(v8);
        vertices.push_back(v9);
        vertices.push_back(v10);
        vertices.push_back(v11);

        indices =  {0, 11,  5,
                    0,  5,  1,
                    0,  1,  7,
                    0,  7, 10,
                    0, 10, 11,

                    1,  5,  9,
                    5, 11,  4,
                    11, 10,  2,
                    10,  7,  6,
                    7,  1,  8,

                    3,  9,  4,
                    3,  4,  2,
                    3,  2,  6,
                    3,  6,  8,
                    3,  8,  9,

                    4,  9,  5,
                    2,  4, 11,
                    6,  2, 10,
                    8,  6,  7,
                    9,  8,  1};

        //for(int h = 0; h<vertices.size(); h++){
        //    std::cout<<"["<<vertices[h].position.x<<","<<vertices[h].position.y<<","<<vertices[h].position.z<<"]"<<std::endl;
        //}

        subdivide(10);
    }

    Icosahedron(int subdivision, glm::vec3 colour) {

        float phi = (1 + glm::sqrt(5.0f)) / 2.0f;
        Vertex v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;

        v0.position = {-1.0f,  phi, 0.0f}; v0.colour = colour;
        v1.position = { 1.0f,  phi, 0.0f}; v1.colour = colour;
        v2.position = {-1.0f, -phi, 0.0f}; v2.colour = colour;
        v3.position = { 1.0f, -phi, 0.0f}; v3.colour = colour;

        v4.position = {0.0f, -1.0f,  phi}; v4.colour = colour;
        v5.position = {0.0f,  1.0f,  phi}; v5.colour = colour;
        v6.position = {0.0f, -1.0f, -phi}; v6.colour = colour;
        v7.position = {0.0f,  1.0f, -phi}; v7.colour = colour;

        v8.position =  { phi, 0.0f, -1.0f};  v8.colour = colour;
        v9.position =  { phi, 0.0f,  1.0f};  v9.colour = colour;
        v10.position = {-phi, 0.0f, -1.0f}; v10.colour = colour;
        v11.position = {-phi, 0.0f,  1.0f}; v11.colour = colour;


        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);

        vertices.push_back(v4);
        vertices.push_back(v5);
        vertices.push_back(v6);
        vertices.push_back(v7);

        vertices.push_back(v8);
        vertices.push_back(v9);
        vertices.push_back(v10);
        vertices.push_back(v11);

        indices =  {0, 11,  5,
                    0,  5,  1,
                    0,  1,  7,
                    0,  7, 10,
                    0, 10, 11,

                    1,  5,  9,
                    5, 11,  4,
                    11, 10,  2,
                    10,  7,  6,
                    7,  1,  8,

                    3,  9,  4,
                    3,  4,  2,
                    3,  2,  6,
                    3,  6,  8,
                    3,  8,  9,

                    4,  9,  5,
                    2,  4, 11,
                    6,  2, 10,
                    8,  6,  7,
                    9,  8,  1};

        //for(int h = 0; h<vertices.size(); h++){
        //    std::cout<<"["<<vertices[h].position.x<<","<<vertices[h].position.y<<","<<vertices[h].position.z<<"]"<<std::endl;
        //}

        subdivide(subdivision);
    }
    //helper functions to generate a sphere.
    void subdivide(int factor){
        int triOffset = 0;

        std::vector<Vertex> vertList = std::vector<Vertex>();
        std::vector<uint32_t> indList = std::vector<uint32_t>();

        //std::cout<<triangleCount<<std::endl;

        for(int i=0; i<indices.size()/3; i++){
            Vertex v0,v1,v2;
            v0 = vertices[indices[3*i]];
            v1 = vertices[indices[3*i+1]];
            v2 = vertices[indices[3*i+2]];

            glm::vec3 u_y = (v1.position-v0.position) * 1.0f/(factor+1.0f);
            glm::vec3 u_x = (v2.position-v0.position) * 1.0f/(factor+1.0f);

            int offset = 0;

            for(int x = 0; x<factor; x++){
                for(int y = 0; y<(factor-x); y++){
                    Vertex v;
                    v.position = glm::normalize(v0.position + float(y)*u_y + float(x)*u_x);
                    v.norm = v.position;
                    v.colour = v0.colour;
                    vertList.push_back(v);

                    indList.push_back(y+offset + triOffset);
                    indList.push_back(y+(factor+2-x)+offset + triOffset);
                    indList.push_back(y+1+offset + triOffset);

                    indList.push_back(y+(factor+2-x)+offset + triOffset);
                    indList.push_back(y+1+(factor+2-x)+offset + triOffset);
                    indList.push_back(y+1+offset + triOffset);
                }
                Vertex vEnd0;
                vEnd0.position = glm::normalize(v0.position + float(factor-x)*u_y + float(x)*u_x);
                vEnd0.norm = vEnd0.position;
                vEnd0.colour = v0.colour;
                vertList.push_back(vEnd0);

                Vertex vEnd1;
                vEnd1.position = glm::normalize(v0.position + float(factor+1-x)*u_y + float(x)*u_x);
                vEnd1.norm = vEnd1.position;
                vEnd1.colour = v0.colour;
                vertList.push_back(vEnd1);

                indList.push_back(factor-x + offset + triOffset);
                indList.push_back((factor-x)+(factor+2-x) + offset + triOffset);
                indList.push_back(factor+1-x + offset + triOffset);

                offset += (factor+2)-x;
            }
            Vertex vEnd0;
            vEnd0.position = glm::normalize(v0.position + float(0)*u_y + float(factor)*u_x);
            vEnd0.norm = vEnd0.position;
            vEnd0.colour = v0.colour;
            vertList.push_back(vEnd0);

            Vertex vEnd1;
            vEnd1.position = glm::normalize(v0.position + float(1)*u_y + float(factor)*u_x);
            vEnd1.norm = vEnd1.position;
            vEnd1.colour = v0.colour;
            vertList.push_back(vEnd1);

            Vertex vEndEnd;
            vEndEnd.position = glm::normalize(v0.position + float(0)*u_y + float(factor+1)*u_x);
            vEndEnd.norm = vEndEnd.position;
            vEndEnd.colour = v0.colour;
            vertList.push_back(vEndEnd);

            //last triangle
            indList.push_back(offset + triOffset);
            indList.push_back(offset+2 + triOffset);
            indList.push_back(offset+1 + triOffset);

            triOffset += ((factor+3)*(factor+2))/2;
        }

        vertices.clear();
        indices.clear();
        vertices = vertList;
        indices = indList;

        /*
        for(int h = 0; h<vertices.size(); h++){
            std::cout<<"position: ["<<vertices[h].position.x<<","<<vertices[h].position.y<<","<<vertices[h].position.z<<"]"<<std::endl;
            std::cout<<"normal : ["<<vertices[h].norm.x<<","<<vertices[h].norm.y<<","<<vertices[h].norm.z<<"]"<<std::endl;
        }*/
    }


};

#endif //VULKANTESTING_ICOSAHEDRON_H
