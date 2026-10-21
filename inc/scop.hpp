#pragma once

//  DEBUG
#ifdef DEBUG
  #define LOG(x) do { std::cout << x << std::endl; } while (0)
#else
  #define LOG(x)
#endif

// DEFINES
#define WIN_WIDTH   1920
#define WIN_HEIGHT  1080

// INCLUDES
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <unistd.h>
#include <cstdint>
#include <iterator>
#include <algorithm>
#include <stdexcept>

// Includes all sub classes/files
#include "Image.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Vec3.hpp"
#include "Mat4.hpp"
#include "Parser.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

//CONFIG
namespace cfg {
    constexpr float PI          = 3.14159265f;
    constexpr float KEY_ROT     = 2.0f;     // rotation q/e
    constexpr float AUTO_ROT    = 0.15f;    // rotation auto
    constexpr float FADE_SPEED  = 0.5f;     // transition speed
    constexpr float FOV_DEG     = 45.0f;
    constexpr float MOUSE_SENS  = 0.005f;   // rotation p. pixel
    constexpr float MOVE_SPEED_REL = 0.5f;
    constexpr float CAM_DISTANCE = 4.0f;
}

struct ObjectState
{
    Vec3    position;               // WASD / R / F
    float   autoRotation  = 0.0f;
    float   yaw           = 0.0f;   // Q/E
    float   blend         = 0.0f;
    float   blendTarget   = 0.0f;
    // mouse
    float   distance      = 0.0f;
    float   startDistance = 0.0f;
    float   zoomStep      = 0.0f;
    float   moveSpeed     = 0.0f;
    bool    dragging      = false;
    double  lastX         = 0.0;
    double  lastY         = 0.0;
    float   pitch         = 0.0f;
    bool    light         = false;  // L
    bool    wireframe     = false;  // Y
};