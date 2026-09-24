#pragma once

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Mat4.hpp"


class Shader
{
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        ~Shader();

        void use() const;
        GLuint getID() const;
        void setMat4(const std::string &name, const Mat4 &mat) const;

    private:
        GLuint _id;
        std::string readFile(const std::string &path);
        GLuint compile(const std::string &source, GLenum type);
        void checkCompileErrors(GLuint shader, const std::string&type);
};
