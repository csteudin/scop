#pragma once

#include "glad/glad.h"
#include <vector>
#include "Parser.hpp"

class Mesh
{
    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
        ~Mesh();

        void draw() const;

    private:
        GLuint _VAO, _VBO, _EBO;
        size_t _indexCount;

        void setupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
};
