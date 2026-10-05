/*  INTELLISENSE .vscode fix
{
  "configurations": [
    {
      "name": "linux-gcc-x64",
      "includePath": [
        "${workspaceFolder}**",
        "${workspaceFolder}/inc",
        "${workspaceFolder}/external/glad/include"
      ],
      "compilerPath": "/usr/bin/gcc",
      "cStandard": "${default}",
      "cppStandard": "${default}",
      "intelliSenseMode": "linux-gcc-x64",
      "compilerArgs": [
        ""
      ]
    }
  ],
  "version": 4
}
*/

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

// Includes all sub classes/files
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
    constexpr float MOVE_SPEED  = 0.025f;     // units per sec
    constexpr float KEY_ROT     = 2.0f;     // rotation q/e
    constexpr float AUTO_ROT    = 0.15f;    // rotation auto
    constexpr float MOUSE_SENS  = 0.005f;   // rotation p. pixel
    constexpr float FADE_SPEED  = 2.0f;     // transition speed
    constexpr float FOV_DEG     = 45.0f;
}

struct ObjectState
{
    Vec3   position;                        // WASD / R / F
    float  autoRotation  = 0.0f;
    float  yaw           = 0.0f;            // Q/E
    float  pitch         = 0.0f;            // 
    float  distance      = 0.0f;            // 
    float  startDistance = 0.0f;
    float  zoomStep      = 0.0f;
    bool   dragging      = false;           // 
    double lastX         = 0.0;
    double lastY         = 0.0;
    float  blend         = 0.0f;            // 
    float  blendTarget   = 0.0f;
};