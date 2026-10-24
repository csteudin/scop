#include "../inc/scop.hpp"

/*  TEXTURE::TEXTURE
    - loads the BMP and uploads it as an OpenGL texture (BGR / BGRA, rows are tightly packed:
      alignment 1)
    - builds mipmaps (smaller copies against flickering); GL_CLAMP_TO_EDGE: nothing is repeated
      outside 0..1
*/
Texture::Texture(const std::string &tex_path)
{
    Image img = loadBMP(tex_path);

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum format   = (img.channels == 4) ? GL_BGRA : GL_BGR;
    GLint  internal = (img.channels == 4) ? GL_RGBA8 : GL_RGB8;
    glTexImage2D(GL_TEXTURE_2D, 0, internal, img.width, img.height, 0, format,
                    GL_UNSIGNED_BYTE, img.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    LOG("! Texture Created");
}

/*  TEXTURE::~TEXTURE
    - deletes the OpenGL texture
*/
Texture::~Texture()
{
    glDeleteTextures(1, &_id);
    LOG("! Texture Destroyed");
}

/*  TEXTURE::BIND
    - puts the texture on a texture unit; the sampler uniform in the shader must get the same
      unit number (setInt)
*/
void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}