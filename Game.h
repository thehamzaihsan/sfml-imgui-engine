#ifndef GAME_H
#define GAME_H
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <fstream>
#include "object.h"

class Game{
    public:
    vector<Object> everything_map;
    void Update();
    void Start();
};

#endif