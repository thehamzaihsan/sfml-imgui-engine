#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <fstream>
#include "../object.h"



void PlayerCl::Start(){
    this->setFillColor(sf::Color::Magenta);
    this->setRadius(20); 
}


void PlayerCl::Update(){

}