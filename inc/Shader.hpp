#pragma once

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Shader
{
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        ~Shader();

        void use() const;
        GLuint getID() const;

    private:
        GLuint _id;
        std::string readFile(const std::string &path);
        GLuint compile(const std::string &source, GLenum type);
        void checkCompileErrors(GLuint shader, const std::string&type);
};
