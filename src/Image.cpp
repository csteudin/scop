#include "../inc/scop.hpp"
#include <cstdint>
#include <stdexcept>

static uint16_t read16(const unsigned char *p)
{
    return static_cast<uint16_t>(p[0] | (p[1] << 8));
}

static uint32_t read32(const unsigned char *p)
{
    return (static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8) | (static_cast<uint32_t>(p[2]) << 16) | (static_cast<uint32_t>(p[3]) << 24));
}

Image loadBMP(const std::string &tex_path)
{
    std::ifstream file(tex_path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("BMP: cannot open " + tex_path);

    std::vector<unsigned char> d((std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>());

    if (d.size() < 54 || d[0] != 'B' || d[1] != 'M')
        throw std::runtime_error("BMP: not a BMP file: " + tex_path);

    uint32_t dataOffset  = read32(&d[10]);
    int32_t  w           = static_cast<int32_t>(read32(&d[18]));
    int32_t  h           = static_cast<int32_t>(read32(&d[22]));
    uint16_t bpp         = read16(&d[28]);
    uint32_t compression = read32(&d[30]);

    bool bitfields = (compression == 3 && bpp == 32 && d.size() >= 66
                        && read32(&d[54]) == 0x00FF0000 && read32(&d[58]) == 0x0000FF00
                        && read32(&d[62]) == 0x000000FF);
    
    if ((compression != 0 && !bitfields) || (bpp != 24 && bpp != 32) || w <= 0 || h == 0)
        throw std::runtime_error("BMP: only uncompressed 24/32 bit supported: " + tex_path);

    bool topDown = (h < 0);
    size_t width = static_cast<size_t>(w);
    size_t height = static_cast<size_t>(topDown ? -h : h);
    size_t bytes = bpp / 8;
    size_t rowBytes = width * bytes;
    size_t rowSize = (rowBytes + 3) / 4 * 4;

    if (dataOffset + rowSize * height > d.size())
        throw std::runtime_error("BMP: file truncated: " + tex_path);

    Image img;

    img.width = w;
    img.height = static_cast<int>(height);
    img.channels = static_cast<int>(bytes);
    img.pixels.resize(rowBytes * height);

    for(size_t row = 0; row < height; row++)
    {
        size_t srcRow = topDown ? (height - 1 - row) : row;
        const unsigned char *src = &d[dataOffset + (srcRow * rowSize)];
        std::copy(src, src + rowBytes, &img.pixels[row * rowBytes]);
    }
    
    return (img);
}
