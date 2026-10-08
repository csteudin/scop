#pragma once

#include <string>
#include <vector>

struct Image
{
    int width = 0;
    int height = 0;
    int channels = 0; // 3<BGR> or 4<BGRA>
    std::vector<unsigned char> pixels;
};

Image loadBMP(const std::string &tex_path);
