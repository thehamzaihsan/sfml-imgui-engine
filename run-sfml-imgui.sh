#!/bin/bash
g++ -o $1 $1.cpp -I/home/hamzaihsan/projects/sfml/imgui-engine-base/imgui -I/home/hamzaihsan/projects/sfml/imgui-engine-base/imgui-sfml -L/home/hamzaihsan/projects/sfml/imgui-engine-base/imgui-sfml/build -lImGui-SFML -lsfml-graphics -lsfml-window -lsfml-system -lGL && ./$1