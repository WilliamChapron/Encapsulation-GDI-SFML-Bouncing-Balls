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
#include "Ennemy.hpp"
#include "Transition.hpp"
#include "Behaviour.hpp"
#include "Condition.hpp"
#include "Action.hpp"


// Rpm
class ShootPerformedCondition : public Condition {
public:
    bool Test(Plant* plant) override {
        //std::cout << "Check Shoot perform condition";
        return plant->isShootPerformed;
    }
};

class CanShootAgainCondition : public Condition {
public:
    bool Test(Plant* plant) override {
        //std::cout << "Check can shoot \n";
        if (!plant->rpmTimer.isInit) {
            return true;
        }

        if (plant->rpmTimer.CheckEndTimer(0.4)) {
            plant->isShootPerformed = false; 
            return true; 
        }
        return false;
    }
};

// Ammo
class NoMoreAmmoCondition : public Condition {
public:
    bool Test(Plant* plant) override {
        if (plant->mAmmoCount < 1) {
            //std::cout << "Check ammo count" << plant->mAmmoCount << "/" << plant->mMaxAmmo;
            plant->reloadingTimer.Start();
            return true;
        }
        return false;
    }
};

class ReloadingEnd : public Condition {
public:
    bool Test(Plant* plant) override {
        if (plant->reloadingTimer.CheckEndTimer(3)) {
            plant->mAmmoCount = plant->mMaxAmmo;
            return true;
        }
        return false;
    }
};

// Check enemy
class CheckEnemy : public Condition {
public:
    bool Test(Plant* plant) override {
        return 1 == 1;
    }
};




class ShootAction : public Action {
public:

    void Start(Plant* plant) override {
    }
    void Update(Plant* plant) override {
        Projectile* projectile = new Projectile(plant->getPosition().x + plant->plantShape.getRadius(), plant->getPosition().y + plant->plantShape.getRadius(), 0.1f, 0.0f, 10.0f);
        plant->balls.push_back(projectile);

        //std::cout << "Ammo :" << plant->mAmmoCount << "/" << plant->mMaxAmmo;
        plant->mAmmoCount--;


        plant->isShootPerformed = true;
        plant->rpmTimer.Start();
        
    }
    void End(Plant* plant) override {
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense");
    int it = 0;
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    Behaviour* behaviour = new Behaviour();
    std::vector<Plant*> allPlants;
    for (int i = 0; i < 4; i++)
    {
        allPlants.push_back(new Plant(sf::Vector2f(10, 50 + i*(window.getSize().y-150.f)/3.f), behaviour, 5));
    }

    std::vector<Ennemy*> allEnnemies;


    Transition* transitionShootToIdle = new Transition();
    transitionShootToIdle->setTargetState(Context::State::Idle);
    ShootPerformedCondition* shootCondition = new ShootPerformedCondition();
    transitionShootToIdle->addCondition(shootCondition);
    behaviour->AddTransition(Context::State::CanShoot, transitionShootToIdle);



    Transition* transitionIdleToShoot = new Transition();
    transitionIdleToShoot->setTargetState(Context::State::CanShoot);

    // Two conditions
    CanShootAgainCondition* idleCondition = new CanShootAgainCondition();
    transitionIdleToShoot->addCondition(idleCondition);
    CheckEnemy* isEnemyInLineCondition = new CheckEnemy();
    transitionIdleToShoot->addCondition(isEnemyInLineCondition);



    behaviour->AddTransition(Context::State::Idle, transitionIdleToShoot);
    //

    // Ammo
    Transition* transitionIdleToReloading = new Transition();
    transitionIdleToReloading->setTargetState(Context::State::Reloading);
    NoMoreAmmoCondition* nomoreammoCondition = new NoMoreAmmoCondition();
    transitionIdleToReloading->addCondition(nomoreammoCondition);

    behaviour->AddTransition(Context::State::Idle, transitionIdleToReloading);


    Transition* transitionReloadingToIdle = new Transition();
    transitionReloadingToIdle->setTargetState(Context::State::Idle);
    ReloadingEnd* reloadingEndCondition = new ReloadingEnd();
    transitionReloadingToIdle->addCondition(reloadingEndCondition);
    behaviour->AddTransition(Context::State::Reloading, transitionReloadingToIdle);
    



    // Shoot
    ShootAction* shootAction = new ShootAction();
    behaviour->AddAction(Context::State::CanShoot, shootAction);


    for (int i = 0; i < 4; i++)
    {
        allPlants[i]->loadTextFont();
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f mouseFloatPosition = window.mapPixelToCoords(mousePosition);
                int line = (int)((mouseFloatPosition.y / window.getSize().y)*4.f);

                allEnnemies.push_back(new Ennemy(sf::Vector2f(mouseFloatPosition.x, 50 + line * (window.getSize().y - 150.f) / 3.f)));
            }
        }
        //std::cout << it << "\n";
        //std::cout << plant->StateToString() << "\n";

        for (int i = 0; i < 4; i++)
        {
            allPlants[i]->Update();
            behaviour->Update(allPlants[i]);
        }

        window.clear();

        for (int i = 0; i < 4; i++) {
            window.draw(allPlants[i]->plantShape);
            window.draw(allPlants[i]->textAmmo);

            // Draw each plant's projectiles (balls)
            int j = 0;
            for (auto ball : allPlants[i]->balls) {
                ball->Update();
                ball->Draw(window);
                j++;
            }
        }


        if (allEnnemies.size() > 0)
        {
            for (int i = 0; i < allEnnemies.size(); i++)
            {
                window.draw(allEnnemies[i]->ennemyShape);
            }
        }

        it++;

        window.display();


    }

    for (int i = 0; i < 4; i++) {
        delete allPlants[i];
    }
    delete behaviour;

    return 0;
}