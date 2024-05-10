#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "object.h"
#include <vector>
#include <fstream>
#include "json.hpp"
using namespace std;
using namespace sf;
using namespace ImGui;

void writeDataToFile(const std::vector<Object> &objects, const std::string &filename)
{
    nlohmann::json j;
    for (const auto &obj : objects)
    {
        nlohmann::json objJson;
        objJson["id"] = obj.id;
        objJson["x"] = obj.getPosition().x;
        objJson["y"] = obj.getPosition().y;
        objJson["width"] = obj.getSize().x;
        objJson["height"] = obj.getSize().y;
        objJson["color"] = obj.getFillColor().toInteger();
        j.push_back(objJson);
    }
    std::ofstream file(filename);
    file << j;
}
int main()
{

    // Start
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game Engine");
    window.setFramerateLimit(60);
    vector<Object> everything_map;
    int curr_id = 00;
    int Selected_Object_id = 1;
    if (!SFML::Init(window))
    {
        std::cout << "SFML::Init failed\n";
        return 1;
    }
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        for (auto &objects : everything_map)
        {
            if (objects.id == Selected_Object_id)
            {
                objects.setOutlineColor(sf::Color::White);
                objects.setOutlineThickness(5);
                objects.update();
            }
            else
            {
                objects.setOutlineThickness(0);
            }
        }

        // IMGUI
        SFML::Update(window, deltaClock.restart());
        ShowDemoWindow();
        SetNextWindowPos(ImVec2(1500, 34), ImGuiCond_FirstUseEver);
        Begin("Properties");
        if (Button("Color Change"))
        {
            for (auto &objects : everything_map)
            {

                if (Selected_Object_id == objects.id)
                {
                    cout << Selected_Object_id;
                    objects.setFillColor(sf::Color::Yellow);
                }
            }
        }
        End();

        Begin("Everything Map");
        for (int i = 0; i < everything_map.size(); i++)
        {
            std::string string_id = std::to_string(everything_map[i].id);
            
            std::string button_label = "Shape" + string_id;
            if (ImGui::Button(button_label.c_str()))
            {
                Selected_Object_id = everything_map[i].id;
            }
          
        }
        if (Button("Add Shape/Block"))
        {
            Object obj;
            obj.id = curr_id++;
            Selected_Object_id = obj.id;
            obj.setPosition(100, 100);
            obj.setSize(sf::Vector2f(100, 100));
            everything_map.push_back({obj});
        }
        End();

        SetNextWindowPos(ImVec2(1500, 10), ImGuiCond_FirstUseEver);
        Begin("Controls");
        if (Button("Play"))
        {
            system("g++ -c game.cpp && g++ game.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system && ./sfml-app");
        }
        if (Button("Save"))
        {

            writeDataToFile(everything_map, "everything.json");
        }
        End();

        // RENDER HERE
        window.clear();
        for (const auto &objects : everything_map)
        {
            window.draw(objects);
        }
        SFML::Render(window);
        window.display();
    }

    SFML::Shutdown();
}
