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

#define WIN_WIDTH   1920
#define WIN_HEIGHT  1080

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
// includes all sub classes/files
#include "Shader.hpp"
#include "Window.hpp"