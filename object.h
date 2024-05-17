#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

using namespace sf;
using namespace std;

class Object : public sf::RectangleShape
{
public:
    bool isPlayer = false;
    bool isEnemy = false;
    bool isWall = false;
    bool isItem = false;
    int id;
    Object()
    {
        setFillColor(sf::Color::Green);
    }
    void update()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            move(-2, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            move(2, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            move(0, -2);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            move(0, 2);
        }
    }
    void render(sf::RenderWindow &window)
    {
        window.draw(*this);
    }
    friend std::istream &operator>>(std::istream &input, Object &obj)
    {
        // Read the necessary data from the input stream and assign it to the object
        // For example, if your .dat file contains x and y coordinates, you can do:
        // input >> obj.x >> obj.y;

        return input;
    }
};

class PlayerCl : public sf::CircleShape
{
public:
    int id = -1;
    void update()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            move(-2, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            move(2, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            move(0, -2);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            move(0, 2);
        }
    }
    friend std::istream &operator>>(std::istream &input, PlayerCl &obj)
    {
        // Read the necessary data from the input stream and assign it to the object
        // For example, if your .dat file contains x and y coordinates, you can do:
        // input >> obj.x >> obj.y;

        return input;
    }
    void Update();
    void Start();
};

#endif // OBJECT_H