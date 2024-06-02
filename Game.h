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

class Game
{
public:
    vector<Object> everything_map;
    bool isTileMap = true;
    void dragObject(std::vector<Object> &objects, int selectedId, const sf::RenderWindow &window)
    {
        static bool isDragging = false;
        static sf::Vector2f offset;

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        for (auto &obj : objects)
        {
            if (obj.id == selectedId)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    if (!isDragging)
                    {
                        // Calculate the offset at the start of dragging
                        offset = obj.getPosition() - window.mapPixelToCoords(mousePosition);
                        isDragging = true;
                    }
                    // Update the position of the object
                    obj.setPosition(window.mapPixelToCoords(mousePosition) + offset);
                }
                else
                {
                    isDragging = false;
                }
                break;
            }
        }
    }
    void Update();
    void Start();
};

#endif