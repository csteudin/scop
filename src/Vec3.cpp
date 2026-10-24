#include "../inc/scop.hpp"

/*  VEC3::VEC3
    - default: the zero vector (0, 0, 0)
*/
Vec3::Vec3() : x(0), y(0), z(0) {}

/*  VEC3::VEC3
    - creates a vector from x, y and z
*/
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}


/*  VEC3::OPERATOR+
    - component-wise addition
*/
Vec3 Vec3::operator+(const Vec3 &other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

/*  VEC3::OPERATOR-
    - component-wise subtraction (a - b points from b to a)
*/
Vec3 Vec3::operator-(const Vec3 &other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

/*  VEC3::OPERATOR*
    - multiplies every component with a number (scales the length)
*/
Vec3 Vec3::operator*(float scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

/*  VEC3::DOT
    - dot product: > 0 same direction, 0 perpendicular, < 0 opposite (used for light and camera)
*/
float Vec3::dot(const Vec3 &other) const
{
    return (x * other.x + y * other.y + z * other.z);
}

/*  VEC3::CROSS
    - cross product: a vector perpendicular to both inputs
    - used for normals (two triangle edges) and for the camera axes in lookAt
*/
Vec3 Vec3::cross(const Vec3 &other) const
{
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

/*  VEC3::LENGTH
    - length of the vector (Pythagoras in 3D)
*/
float Vec3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

/*  VEC3::NORMALIZE
    - same direction, length 1
    - the zero vector is returned unchanged instead of dividing by 0
*/
Vec3 Vec3::normalize() const
{
    float len = length();
    if (len == 0.0f)
        return Vec3(0, 0, 0);
    return Vec3(x / len, y / len, z / len);
}
