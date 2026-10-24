#include "../inc/scop.hpp"

/*  CAMERA::CAMERA
    - stores where the camera stands and what it looks at; "up" is fixed to +Y
*/
Camera::Camera(const Vec3 &position, const Vec3 &target)
    : position(position), target(target), _up(0.0f, 1.0f, 0.0f)
{
}

/*  CAMERA::GETVIEWMATRIX
    - builds the view matrix (lookAt) from the current position and target
    - called every frame, because the zoom moves the camera position
*/
Mat4 Camera::getViewMatrix() const
{
    return Mat4::lookAt(position, target, _up);
}