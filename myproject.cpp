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

// Function that returns a random long long int value
#include <random>

long long int randomValue()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<long long int> dis(0, 9999999999999999);
    return dis(gen);
}

// Function to write object data to a file in JSON format
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
        objJson["isWall"] = obj.isWall;
        objJson["isEnemy"] = obj.isEnemy;
        objJson["isItem"] = obj.isItem;

        j.push_back(objJson);
    }
    std::ofstream file(filename);
    file << j;
}

// Function to read object data from a file in JSON format
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
        sf::Vector2f size(objJson["width"], objJson["height"]);
        obj.setSize(size);
        obj.setFillColor(sf::Color(objJson["color"]));
        obj.isEnemy = objJson["isEnemy"];
        obj.isWall = objJson["isWall"];
        obj.isItem = objJson["isItem"];
        objects.push_back(obj);
    }

    return objects;
}

// Function to save player data to a file in JSON format
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

// Function to read player data from a file in JSON format
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
    // Start
    Game game;
    std::cout << "ImGui version: " << IMGUI_VERSION << std::endl;
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

    std::ifstream file("everything.json");
    if (file.peek() == std::ifstream::traits_type::eof())
    {
        // The file is empty
        std::cerr << "File is empty" << std::endl;
    }
    else
    {
        // The file is not empty, parse it
        game.everything_map = readDataFromFile("everything.json");
    }

    ImGui::SFML::Init(window);
    std::cout << "ImGui-SFML initialized\n";

    // Create ImGui context
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImFont *font = io.Fonts->AddFontFromFileTTF("Rubik-Regular.ttf", 23.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    ImGui::SFML::UpdateFontTexture();

    if (font == NULL || !font->IsLoaded())
    {
        std::cout << "Failed to load font\n";
        return 1;
    }

    std::cout << "Font loaded successfully\n";
    game.Start();
    int Selected_Object_id = 0;

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
        if (Selected_Object_id == -1)
        {
            Player.update();
        }

        for (auto &objects : game.everything_map)
        {
            if (objects.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && event.type == sf::Event::MouseButtonPressed)
            {
                Selected_Object_id = objects.id;
            }
        }

        game.dragObject(game.everything_map, Selected_Object_id, window);

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
        if (TreeNode("Object Color")) // Replace BeginTabItem with TreeNode
        {
            for (auto &objects : game.everything_map)
            {
                if (objects.id == Selected_Object_id)
                {

                    ImVec4 color = ImVec4(objects.getFillColor().r / 255.0f, objects.getFillColor().g / 255.0f, objects.getFillColor().b / 255.0f, objects.getFillColor().a / 255.0f);
                    if (ColorPicker4("Object Color", &color.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
                    {
                        objects.setFillColor(sf::Color(color.x * 255, color.y * 255, color.z * 255, color.w * 255));
                        unsaved = true;
                    }
                }
            }
            TreePop(); // Replace EndTabItem with TreePop
        }

        for (auto &objects : game.everything_map)
        {
            if (Selected_Object_id == objects.id)
            {
                Checkbox("isItem", &objects.isItem);
            }

            if (Selected_Object_id == objects.id)
            {
                if (TreeNode("Object Position")) // Check if TreeNode returns true
                {
                    float posX = objects.getPosition().x;
                    float posY = objects.getPosition().y;
                    ImGui::InputFloat("X", &posX, 0.1f);
                    ImGui::InputFloat("Y", &posY, 0.1f);
                    objects.setPosition(posX, posY);

                    TreePop(); // Only call TreePop if TreeNode returned true
                }
            }
            if (Selected_Object_id == objects.id)
            {
                if (TreeNode("Object Size")) // Check if TreeNode returns true
                {
                    float width = objects.getSize().x;
                    float height = objects.getSize().y;
                    ImGui::InputFloat("Width", &width, 0.001f);
                    ImGui::InputFloat("Height", &height, 0.001f);
                    objects.setSize(sf::Vector2f(width, height));

                    TreePop(); // Only call TreePop if TreeNode returned true
                }
            }
        }

        End();

        Begin("Everything Map", &t, allwins);
        BeginGroup();

        if (ImGui::Button("Delete"))
        {
            if (Selected_Object_id != -1)
            {
                cout << ("Deleted Object with id: " + std::to_string(Selected_Object_id)) << endl;
                game.everything_map.erase(game.everything_map.begin() + Selected_Object_id);
            }
        }

        SameLine();

        if (Button("Add Shape/Blocks"))
        {
            Object obj;
            obj.id = randomValue();
            Selected_Object_id = obj.id;
            obj.setPosition(100, 100);
            obj.setSize(sf::Vector2f(100, 100));
            game.everything_map.push_back({obj});
        }

        EndGroup();

        ImGui::BeginGroup();
        if (ImGui::Selectable("Player", Selected_Object_id == -1))
        {
            Selected_Object_id = -1;
        }
        for (int i = 0; i < game.everything_map.size(); i++)
        {
            std::string string_id = std::to_string(game.everything_map[i].id);
            std::string button_label = "Shape-" + string_id;
            if (ImGui::Selectable(button_label.c_str(), Selected_Object_id == game.everything_map[i].id))
            {
                Selected_Object_id = game.everything_map[i].id;
            }
        }
        ImGui::EndGroup();

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
