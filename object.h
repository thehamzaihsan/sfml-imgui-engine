#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <cmath>
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
    sf::Texture texture;
    string texturePath;
    void setTextureSF(std::string path)
    {
        texturePath = path;
        cout << "Loading texture from path: " << path << endl;
        try
        {
            texture.loadFromFile(path);
            if (!texture.loadFromFile(path))
            {
                throw std::runtime_error("Failed to load texture from file: " + path);
            }
            this->setTexture(&texture);
            

        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    int id;
    Object()
    {
        setFillColor(sf::Color::Green);
    }
    string getPath() const {
        return texturePath;
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
        cout << "hekllo" << this->texturePath << endl;
    }
    friend std::istream &operator>>(std::istream &input, Object &obj)
    {
        // Read the necessary data from the input stream and assign it to the object
        // For example, if your .dat file contains x and y coordinates, you can do:
        // input >> obj.x >> obj.y;

        return input;
    }
     void setPositionRounded(sf::Vector2f pos)
    {
        setPosition(round(pos.x / 64) * 64, round(pos.y / 64) * 64);
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

    //make a function that wraps setPosition but it makes so that every object has a position rounded of to nearest multiple of 64
   

};

#endif // OBJECT_H