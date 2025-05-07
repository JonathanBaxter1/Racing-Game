#!/bin/sh

g++ ./src/*.cpp -o ./bin/race -march=native -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor -lassimp
