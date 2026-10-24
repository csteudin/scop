#include "../inc/scop.hpp"

/*  MESH::MESH
    - remembers how many indices to draw and uploads all data to the GPU (setupMesh)
*/
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    _indexCount = indices.size();
    setupMesh(vertices, indices);
    LOG("! Mesh Created");
}

/*  MESH::~MESH
    - frees the 3 OpenGL objects (VAO, VBO, EBO)
*/
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    LOG("! Mesh Destroyed");
}

/*  MESH::DRAW
    - binds the VAO and draws all triangles with one indexed call; the VAO remembers buffers and
      layout
*/
void Mesh::draw() const
{
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}

/*  MESH::SETUPMESH
    - creates VAO / VBO / EBO and copies the vertices and indices into the GPU buffers
    - describes the Vertex layout: 0 = position, 1 = uv, 2 = normal, 3 = color (same numbers as
      "layout (location = N)" in the vertex shader)
*/
void Mesh::setupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

}