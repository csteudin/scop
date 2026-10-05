#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "Vertex.hpp"

class Parser    {
    public:
        Parser(const std::string &path);

        const std::vector<Vertex> &getVertices() const;
        const std::vector<unsigned int> &getIndices() const;
        const BoundingBox &getBounds() const;

    private:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
        BoundingBox _bounds;

        std::vector<Vec3> _positions;
        std::vector<Vec2> _uvs;
        std::vector<Vec3> _normals;

        void parseLine(const std::string &line);
        void parseFace(std::istringstream &iss);
        void computeBounds();
        void generateUVs();
};
