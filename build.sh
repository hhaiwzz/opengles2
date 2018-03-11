#!/bin/sh

files="es2_triangles.cpp SimpleShader.cpp"
buildname="es2_triangles"

g++ -std=c++14 $files -o $buildname -lGLESv2 -lglfw
./$buildname