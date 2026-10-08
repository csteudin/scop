#include "../inc/scop.hpp"
#include <exception>
#include <sstream>
#include <stdexcept>

static size_t resolveIndex(int idx, size_t size)
{
    long resolved = (idx > 0) ? idx - 1 : static_cast<long>(size) + idx;

    if (idx == 0 || resolved < 0 || resolved >= static_cast<long>(size))
        throw std::runtime_error("OBJ: index out of range");

    return (static_cast<size_t>(resolved));
}

Parser::Parser(const std::string &path)
{
    LOG("! Parser Created");
    
    std::ifstream file(path);
    if(!file.is_open())
        throw std::runtime_error("file does not open");

    std::string line;
    size_t line_num = 0;

    while(std::getline(file, line))
    {
        ++line_num;
        try { parseLine(line); }
        catch (std::exception &e)
        {
            throw std::runtime_error("line " + std::to_string(line_num) + ": " + e.what());
        }
    }
    
    if (_vertices.empty())
        throw std::runtime_error("OBJ: no faces found");
    
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

    computeBounds();

    if (_uvs.empty())
        generateUVs();

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

const BoundingBox &Parser::getBounds() const
{
    return (_bounds);
}

//private
void Parser::parseLine(const std::string &line)
{
    std::istringstream iss(line);

    std::string prefix;
    iss >> prefix;

    if (prefix == "v")
    {
        float x = 0, y = 0, z = 0;
        iss >> x >> y >> z;
        _positions.push_back(Vec3(x, y, z));
    }
    else if (prefix == "vt")
    {
        float x = 0, y = 0;
        iss >> x >> y;
        _uvs.push_back(Vec2(x, y));
    }
    else if (prefix == "vn")
    {
        float x = 0, y = 0, z = 0;
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


    // if (colored == true)
    // {
    //     Vec3 faceColor(  //change later
    //         static_cast<float>(rand()) / RAND_MAX,
    //         static_cast<float>(rand()) / RAND_MAX,
    //         static_cast<float>(rand()) / RAND_MAX
    //     );
    // }
    // else{   }
    

    float g = 0.25f + (0.65f * (static_cast<float>(rand()) / RAND_MAX));
    Vec3 faceColor(g, g, g);

    while(iss >> token)
    {
        std::istringstream tokenStream(token);
        std::string posStr, uvStr, normStr;

        std::getline(tokenStream, posStr, '/');
        std::getline(tokenStream, uvStr, '/');
        std::getline(tokenStream, normStr, '/');

        Vertex v ;

        if (posStr.empty())
            throw std::runtime_error("OBJ: face line is without value !");
        v.position = _positions[resolveIndex(std::stoi(posStr), _positions.size())];

        if (!uvStr.empty())
            v.uv = _uvs[resolveIndex(std::stoi(uvStr), _uvs.size())];

        if (!normStr.empty())
            v.normal = _normals[resolveIndex(std::stoi(normStr), _normals.size())];

        v.color = faceColor;
        
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

void Parser::computeBounds()
{
    _bounds.min = _vertices[0].position;
    _bounds.max = _vertices[0].position;

    for (const auto &v : _vertices)
    {
        _bounds.min.x = std::min(_bounds.min.x, v.position.x);
        _bounds.min.y = std::min(_bounds.min.y, v.position.y);
        _bounds.min.z = std::min(_bounds.min.z, v.position.z);
        _bounds.max.x = std::max(_bounds.max.x, v.position.x);
        _bounds.max.y = std::max(_bounds.max.y, v.position.y);
        _bounds.max.z = std::max(_bounds.max.z, v.position.z);
    }
}

void Parser::generateUVs()
{
    float size = _bounds.maxExtent();
    if (size == 0.0f)
        size = 1.0f;

    for (size_t i = 0; i + 2 < _indices.size(); i += 3)
    {
        Vertex &v0 = _vertices[_indices[i]];
        Vertex &v1 = _vertices[_indices[i + 1]];
        Vertex &v2 = _vertices[_indices[i + 2]];

        Vec3 n = (v1.position - v0.position).cross(v2.position - v0.position);
        float ax = std::fabs(n.x), ay = std::fabs(n.y), az = std::fabs(n.z);

        for (Vertex *v : { &v0, &v1, &v2 })
        {
            Vec3 p = v->position - _bounds.min;
            if (ax >= ay && ax >= az)
                v->uv = Vec2(p.z / size, p.y / size);      // Seite zeigt in X
            else if (ay >= az)
                v->uv = Vec2(p.x / size, p.z / size);      // Seite zeigt in Y
            else
                v->uv = Vec2(p.x / size, p.y / size);      // Seite zeigt in Z
        }
    }
}
