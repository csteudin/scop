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