#include "../inc/scop.hpp"

/*  MAT4::MAT4
    - creates the identity matrix (1 on the diagonal, 0 elsewhere): multiplying with it changes
      nothing
    - all matrices are row-major, they are uploaded with GL_TRUE (transpose) in Shader::setMat4
*/
Mat4::Mat4()
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m[row][col] = 0.0f;
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
}

/*  MAT4::OPERATOR*
    - matrix product: every field = row of the left matrix times column of the right matrix
    - the order matters: (A * B) applies B FIRST, that is why we write proj * view * model
*/
Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
                sum += m[row][k] * other.m[k][col];
            result.m[row][col] = sum;
        }
    }
    return result;
}

/*  MAT4::IDENTITY
    - returns a fresh identity matrix
*/
Mat4 Mat4::identity()
{
    return Mat4();
}

/*  MAT4::TRANSLATE
    - moves everything by t: the offset sits in the last column
*/
Mat4 Mat4::translate(const Vec3 &t)
{
    Mat4 result;
    result.m[0][3] = t.x;
    result.m[1][3] = t.y;
    result.m[2][3] = t.z;
    return result;
}

/*  MAT4::SCALE
    - stretches / shrinks by s on every axis (the diagonal)
*/
Mat4 Mat4::scale(const Vec3 &s)
{
    Mat4 result;
    result.m[0][0] = s.x;
    result.m[1][1] = s.y;
    result.m[2][2] = s.z;
    return result;
}

/*  MAT4::ROTATEX
    - rotates around the X axis by "radians" (used for the up / down tilt: pitch)
*/
Mat4 Mat4::rotateX(float radians)
{
    Mat4 result;
    float c = std::cos(radians);
    float s = std::sin(radians);

    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;

    return result;
}

/*  MAT4::ROTATEY
    - rotates around the Y axis by "radians" (used for auto rotation and left / right turn: yaw)
*/
Mat4 Mat4::rotateY(float radians)
{
    Mat4 result;
    float c = std::cos(radians);
    float s = std::sin(radians);

    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;

    return result;
}

/*  MAT4::ROTATEZ
    - rotates around the Z axis by "radians"
*/
Mat4 Mat4::rotateZ(float radians)
{
    Mat4 result;
    float c = std::cos(radians);
    float s = std::sin(radians);

    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;

    return result;
}

/*  MAT4::PERSPECTIVE
    - projection matrix: the visible volume (fov, aspect, near / far plane) becomes the OpenGL
      clip space
    - the -1 in m[3][2] copies -z into w, the later divide by w makes far things smaller
*/
Mat4 Mat4::perspective(float fovRadians, float aspect, float near, float far)
{
    Mat4 result;
    float f = 1.0f / std::tan(fovRadians / 2.0f);

    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    result.m[2][2] = (far + near) / (near - far);
    result.m[2][3] = (2.0f * far * near) / (near - far);
    result.m[3][2] = -1.0f;
    result.m[3][3] = 0.0f;

    return result;
}

/*  MAT4::LOOKAT
    - view matrix: turns the world so that the camera (eye) sits at the origin and looks at the
      target
    - forward / right / newUp are the camera axes (rows); the last column moves the eye to the
      origin
*/
Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up)
{
    Vec3 forward = (target - eye).normalize();
    Vec3 right = forward.cross(up).normalize();
    Vec3 newUp = right.cross(forward);

    Mat4 result;

    result.m[0][0] = right.x;
    result.m[0][1] = right.y;
    result.m[0][2] = right.z;

    result.m[1][0] = newUp.x;
    result.m[1][1] = newUp.y;
    result.m[1][2] = newUp.z;

    result.m[2][0] = -forward.x;
    result.m[2][1] = -forward.y;
    result.m[2][2] = -forward.z;

    result.m[0][3] = -right.dot(eye);
    result.m[1][3] = -newUp.dot(eye);
    result.m[2][3] = forward.dot(eye);

    return result;
}