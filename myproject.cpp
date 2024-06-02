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
#include "./scripts/playerScript.h"
#include "Game.h"
#include "./scripts/manageScript.h"

using namespace std;
using namespace sf;
using namespace ImGui;

// a function that returns a long long int of random value it should keep time as a seed
long long int randomValue()
{
    // change so this ID is 16 digit long and includes a letters and numbers
    srand(time(0));
    long long int id = rand() % 10000000000000000;
    return id;
}

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
        // add booleans
        objJson["isWall"] = obj.isWall;
        objJson["isEnemy"] = obj.isEnemy;
        objJson["isItem"] = obj.isItem;

        j.push_back(objJson);
    }
    std::ofstream file(filename);
    file << j;
}
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

void SavePlayerData(const PlayerCl &player, const std::string &filename)
{
    nlohmann::json j;
    j["id"] = player.id;
    j["x"] = player.getPosition().x;
    j["y"] = player.getPosition().y;
    j["radius"] = player.getRadius();
    j["color"] = player.getFillColor().toInteger();
    std::ofstream file(filename);
    file << j;
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

void dragObject(std::vector<Object>& objects, int selectedId, const sf::RenderWindow& window) {
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

int main()
{

    // Start
    Game game;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game Engine");
    window.setFramerateLimit(60);
    PlayerCl Player;
    Player.setFillColor(Color::Blue);
    Player.setRadius(50);
    Player.setPosition(100, 100);
    Player.setOutlineColor(Color::White);
    Player.setOutlineThickness(0);
    bool unsaved = true;
    ImGuiWindowFlags allwins = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    ImGuiWindowFlags controlWins = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    if (unsaved)
    {
        controlWins = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_UnsavedDocument;
    }
    if (readDataFromFile("everything.json").size() != 0)
    {
        game.everything_map = readDataFromFile("everything.json");
    }

    ImGui::SFML::Init(window);

    std::cout << "ImGui-SFML initialized\n";

    // Create ImGui context
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImFont *font = io.Fonts->AddFontFromFileTTF("Geist-Regular.ttf", 23.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    ImGui::SFML::UpdateFontTexture();

    if (font == NULL)
    {
        std::cout << "Failed to load font\n";
        return 1;
    }
    else if (!font->IsLoaded())
    {
        std::cout << "Font is not loaded\n";
        return 1;
    }

    std::cout << "Font loaded successfully\n";
    game.Start();
    int Selected_Object_id = 0;
    // Update font texture
    // if (font == NULL)
    // {
    //     std::cout << "Failed to load font\n";
    //     return 1;
    // }
    // else if (!font->IsLoaded())
    // {
    //     std::cout << "Font is not loaded\n";
    //     return 1;
    // }

    if (unsaved)
    {
        controlWins = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_UnsavedDocument;
    }
    else
    {
        controlWins = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    }

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    Player.Start();

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

        for (auto &objects : game.everything_map)
        {
            if (objects.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && event.type == sf::Event::MouseButtonPressed)
            {
                Selected_Object_id = objects.id;
            }
        }

        dragObject(game.everything_map, Selected_Object_id, window);

        if (Selected_Object_id == -1)
        {
            Player.update();
        }
        for (auto &objects : game.everything_map)
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
        SetNextWindowPos(ImVec2(1500, 34), ImGuiCond_FirstUseEver);
        PushFont(font);
        ShowDemoWindow();
        bool t = true;
        Begin("Properties", &t, allwins);

        // ImGui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

        if (Button("Color Change"))
        {
            for (auto &objects : game.everything_map)
            {

                if (Selected_Object_id == objects.id)
                {
                    cout << Selected_Object_id;
                    objects.setFillColor(sf::Color::Yellow);
                    *&unsaved = true;
                }
            }
        }
        for (auto &objects : game.everything_map)
        {

            if (Selected_Object_id == objects.id)
            {
                Checkbox("isCollidable", &objects.isWall);
            }
        }
        for (auto &objects : game.everything_map)
        {

            if (Selected_Object_id == objects.id)
            {
                Checkbox("isItem", &objects.isItem);
            }
        }
        End();

        Begin("Everything Map", &t, allwins);
        BeginGroup();
        if (ImGui::Button("Delete"))
        {
            game.everything_map.erase(game.everything_map.begin() + Selected_Object_id);
        }
        SameLine();
        if (Button("Add Shape/Block"))
        {
            Object obj;
            obj.id = randomValue();
            Selected_Object_id = obj.id;
            obj.setPosition(100, 100);
            obj.setSize(sf::Vector2f(100, 100));
            game.everything_map.push_back({obj});
        }
        EndGroup();
        for (int i = 0; i < game.everything_map.size(); i++)
        {
            std::string string_id = std::to_string(game.everything_map[i].id);
            ImGui::BeginGroup();

            std::string button_label = "Shape" + string_id;
            if (ImGui::Button(button_label.c_str()))
            {
                Selected_Object_id = game.everything_map[i].id;
            }

            ImGui::EndGroup();
        }
        if (ImGui::Button("Player"))
        {
            Selected_Object_id = -1;
        }

        End();

        SetNextWindowPos(ImVec2(1500, 10), ImGuiCond_FirstUseEver);
        Begin("Controls", &t, controlWins);
        BeginGroup();
        {
            if (Button("Save & Play"))
            {
                SavePlayerData(Player, "player.json");
                unsaved = false;
                writeDataToFile(game.everything_map, "everything.json");
                system("g++ -c game.cpp && g++ game.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system && ./sfml-app");
            }
            SameLine();
            if (Button("Play"))
            {
                system("g++ -c game.cpp && g++ game.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system && ./sfml-app");
            }
            SameLine();
            if (Button("Save"))
            {
                SavePlayerData(Player, "player.json");
                unsaved = false;
                writeDataToFile(game.everything_map, "everything.json");
            }
        }
        EndGroup();
        End();
        PopFont();
        // RENDER HERE
        window.clear();
        for (const auto &objects : game.everything_map)
        {
            window.draw(objects);
        }
        window.draw(Player);
        SFML::Render(window);
        window.display();
    }

    SFML::Shutdown();
}

// TodoList