#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "object.h"
#include <iostream>
#include "json.hpp"
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
        objects.push_back(obj);
    }

    return objects;
}

int main()
{
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Tile Map");

    vector<Object> everything_map;
    everything_map =  readDataFromFile("everything.json");

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
           
            
        }

        // Clear the window
        window.clear();

        // Render the tile map
        for (const auto &obj : everything_map)
        {
           window.draw(obj);
        }

        // Display the window
        window.display();
    }

    return 0;
}