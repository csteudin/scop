#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"

class Camera
{
    public:
        Camera(const Vec3 &position, const Vec3 &target);

        Mat4 getViewMatrix() const;

        Vec3 position;
        Vec3 target;

    private:
        Vec3 _up;
};
