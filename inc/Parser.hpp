#pragma once

#include <Vec3.hpp>

struct Vec2
{
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
};

struct Vertex
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
};

class Parser    {
    public:
        Parser(const std::string &path);

        const std::vector<Vertex> &getVertices() const;
        const std::vector<unsigned int> &getIndices() const;

    private:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;

        std::vector<Vec3> _positions;
        std::vector<Vec2> _uvs;
        std::vector<Vec3> _normals;

        void parseLine(const std::string &line);
        void parseFace(std::istringstream &iss);
};
