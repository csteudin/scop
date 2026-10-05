#pragma once

#include <algorithm>
#include "Vec3.hpp"

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
    Vec3 color;
};
static_assert(sizeof(Vertex) == 11 * sizeof(float), "Vertex has Padding: Layout is incorrect");

struct BoundingBox
{
    Vec3 min, max;

    Vec3  center() const { return (min + max) * 0.5f; }
    float radius() const { return (max - min).length() * 0.5f; }
    float maxExtent() const
    {
        Vec3 d = max - min;
        return std::max(d.x, std::max(d.y, d.z));
    }
};