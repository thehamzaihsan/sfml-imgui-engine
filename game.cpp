#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "object.h"
#include <iostream>
#include "json.hpp"
#include "./scripts/playerScript.h"
#include "Game.h"
#include "./scripts/manageScript.h"
using namespace std;

std::vector<Object> readDataFromFile(const std::string &filename)
{
    std::vector<Object> objects;

    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;

    for (const auto &objJson : j)
    {
        if (!objJson.is_object())
        {
            std::cerr << "Invalid JSON: expected an object, got " << objJson << std::endl;
            continue;
        }

        Object obj;
        obj.id = objJson["id"];
        obj.setPosition(objJson["x"], objJson["y"]);
        sf::Vector2f size(objJson["width"], objJson["height"]); // Convert values to sf::Vector2f
        obj.setSize(size);
        obj.setFillColor(sf::Color(objJson["color"]));
        obj.isEnemy = objJson["isEnemy"];
        obj.isWall = objJson["isWall"];
        obj.isItem = objJson["isItem"];
        objects.push_back(obj);
    }

    return objects;
}



PlayerCl readPlayerJson(const std::string &filename)
{
    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;
    PlayerCl player;
    player.id = j["id"];
    player.setPosition(j["x"], j["y"]);
    player.setRadius(j["radius"]);
    player.setFillColor(sf::Color(j["color"]));
    return player;
}

int main()
{
    // Create the SFML window
    Game game;
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Tile Map");
    PlayerCl Player;
    game.everything_map;
    game.everything_map =  readDataFromFile("everything.json");
    Player = readPlayerJson("player.json");
    game.Start();
    Player.Start();
    // Main game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
           Player.Update();
            
        }

        // Clear the window
        window.clear();

        // Render the tile map
        for (const auto &obj : game.everything_map)
        {
           window.draw(obj);
        }
        window.draw(Player);
        // Display the window
        window.display();
    }

    return 0;
}