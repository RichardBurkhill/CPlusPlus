#!/bin/bash
set -e

# Compile your OpenGL app
clang++ OpenGL.cpp /Users/richardburkhill/dev/glad/src/gl.c \
  -I/Users/richardburkhill/dev/glad/include \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lglfw -framework OpenGL \
  -o OpenGLApp