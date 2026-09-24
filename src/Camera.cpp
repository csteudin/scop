#include "../inc/scop.hpp"

Camera::Camera(const Vec3 &position, const Vec3 &target)
    : position(position), target(target), _up(0.0f, 1.0f, 0.0f)
{
}

Mat4 Camera::getViewMatrix() const
{
    return Mat4::lookAt(position, target, _up);
}