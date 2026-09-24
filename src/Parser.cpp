#include "../inc/scop.hpp"
#include <exception>
#include <sstream>
#include <stdexcept>

Parser::Parser(const std::string &path)
{
    LOG("! Parser Created");
    std::ifstream file(path);
    if(!file.is_open())
        throw std::runtime_error(".obj file doesnt open");

    std::string line;
    while(std::getline(file, line))
    {
        parseLine(line);
    }
    if (_normals.empty())
    {
        for (size_t i = 0; i + 2 < _indices.size(); i += 3)
        {
            Vertex &v0 = _vertices[_indices[i]];
            Vertex &v1 = _vertices[_indices[i + 1]];
            Vertex &v2 = _vertices[_indices[i + 2]];

            Vec3 edge1 = v1.position - v0.position;
            Vec3 edge2 = v2.position - v0.position;
            Vec3 normal = edge1.cross(edge2).normalize();

            v0.normal = normal;
            v1.normal = normal;
            v2.normal = normal;
        }
    }
    LOG("Vertices: " << _vertices.size());
    LOG("Indices: " << _indices.size());
    LOG("Triangles: " << _indices.size() / 3);
}

const std::vector<Vertex> &Parser::getVertices() const
{
    return (_vertices);
}

const std::vector<unsigned int> &Parser::getIndices() const
{
    return (_indices);
}

void Parser::parseLine(const std::string &line)
{
    std::istringstream iss(line);

    std::string prefix;
    iss >> prefix;

    if (prefix == "v")
    {
        float x, y, z;
        iss >> x >> y >> z;
        _positions.push_back(Vec3(x, y, z));
    }
    else if (prefix == "vt")
    {
        float x, y;
        iss >> x >> y;
        _uvs.push_back(Vec2(x, y));
    }
    else if (prefix == "vn")
    {
        float x, y, z;
        iss >> x >> y >> z;
        _normals.push_back(Vec3(x, y, z));
    }
    else if (prefix == "f")
    {
        parseFace(iss);
    }
    else
        return ;
}

void Parser::parseFace(std::istringstream &iss)
{
    std::vector<unsigned int> faceIndices;
    std::string token;

    while(iss >> token)
    {
        std::istringstream tokenStream(token);
        std::string posStr, uvStr, normStr;

        std::getline(tokenStream, posStr, '/');
        std::getline(tokenStream, uvStr, '/');
        std::getline(tokenStream, normStr, '/');

        Vertex v ;

        int posIndex = std::stoi(posStr) - 1;
        v.position = _positions[posIndex];

        if (!uvStr.empty())
        {
            int uvIndex = std::stoi(uvStr) - 1;
            v.uv = _uvs[uvIndex];
        }

        if (!normStr.empty())
        {
            int normIndex = std::stoi(normStr) - 1;
            v.normal = _normals[normIndex];
        }
        
        _vertices.push_back(v);
        faceIndices.push_back(_vertices.size() - 1);
    }
    
    for (size_t i = 1; i + 1 < faceIndices.size(); i++)
    {
        _indices.push_back(faceIndices[0]);
        _indices.push_back(faceIndices[i]);
        _indices.push_back(faceIndices[i + 1]);
    }
}