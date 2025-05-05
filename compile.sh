#!/bin/sh

g++ main.cpp model.cpp mesh.cpp models.cpp utils.cpp scene.cpp -o race -march=native -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor -lassimp
#g++ main.cpp utils.cpp -o race -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor -lassimp
