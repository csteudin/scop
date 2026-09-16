#include "../inc/scop.hpp"

Parser::ObjParser(const std::string &path)
{
    (void)path;
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
    (void)line;
}

void Parser::parseFace(std::istringstream &iss)
{
    (void)iss;
}