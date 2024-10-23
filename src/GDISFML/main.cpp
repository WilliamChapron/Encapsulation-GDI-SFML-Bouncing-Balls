#include "pch.h"
//#include "App.h"
//#include "Window.h"
//#include "GDIWindow.h"
//#include "Texture.h"
//#include "Text.h"
//#include "BouncingBall.h"
//#include "./CommonStructures.h"

#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif


#ifdef SFML_MODE
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#endif

#include <windows.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <chrono>

#include <chrono>
#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <functional> 

using std::string;

#define COOLPURPLE \
    CLITERAL(Color) { 153, 0, 0, 255 }  // cool Purple

constexpr int G_RELOAD_TIME = 3;

#include <SFML/Graphics.hpp>
#include "Plant.hpp"
#include "Transition.hpp"
#include "Behaviour.hpp"
#include "Condition.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense");

    Behaviour* behaviour = new Behaviour();
    Plant* plant = new Plant(sf::Vector2f(100, 100), behaviour, 5);

    Transition* transition = new Transition();
    transition->setTargetState(Context::State::SomeState);
    behaviour->AddTransition(Context::State::SomeState, transition);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        plant->Update();
        behaviour->Update(plant);

        window.clear();

        sf::RectangleShape plantShape(sf::Vector2f(50, 50));
        plantShape.setFillColor(plant->getColor());
        plantShape.setPosition(plant->getPosition());
        window.draw(plantShape);

        window.display();
    }

    delete plant;
    delete behaviour;

    return 0;
}
