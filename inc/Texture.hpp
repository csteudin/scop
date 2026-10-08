#pragma once

#include "glad/glad.h"
#include <string>

class Texture
{
    public:
        Texture(const std::string &tex_path);
        ~Texture();

        void bind(unsigned int uint = 0) const;

    private:
        GLuint _id;
};