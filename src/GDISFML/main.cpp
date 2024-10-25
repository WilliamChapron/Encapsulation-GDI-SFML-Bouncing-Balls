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
#include <math.h>

#include <thread>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <functional> 

using std::string;

#define COOLPURPLE \
    CLITERAL(Color) { 153, 0, 0, 255 }  // cool Purple

constexpr int G_RELOAD_TIME = 5;

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

        if (plant->rpmTimer.CheckEndTimer(0.6)) {
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
        if (plant->reloadingTimer.CheckEndTimer(G_RELOAD_TIME)) {
            plant->mAmmoCount = plant->mMaxAmmo;
            return true;
        }
        return false;
    }
};

// Check enemy
class CheckEnnemy : public Condition {
public:
    // Ajout de la liste d'ennemis à vérifier pour chaque plante
    std::vector<Ennemy*>* mEnemies;

    CheckEnnemy(std::vector<Ennemy*>* enemies) : mEnemies(enemies) {}

    bool Test(Plant* plant) override {
        for (auto& enemy : *mEnemies) {
            // Vérifier si l'ennemi est dans la même ligne que la plante (avec une marge pour tolérance)
            if (std::abs(enemy->getPosition().y - plant->getPosition().y) < 20.0f) {
                return true;
            }
        }
        return false; // Aucun ennemi dans la ligne
    }
};




class ShootAction : public Action {
public:

    void Start(Plant* plant) override {
    }
    void Update(Plant* plant) override {
        Projectile* projectile = new Projectile(plant->getPosition().x + plant->plantShape.getRadius(), plant->getPosition().y , 0.1f, 0.0f, 10.0f);
        
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
        allPlants.push_back(new Plant(sf::Vector2f(30, 50 + i*(window.getSize().y-150.f)/3.f), behaviour, 5));
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
    CheckEnnemy* isEnnemyInLineCondition = new CheckEnnemy(&allEnnemies);
    transitionIdleToShoot->addCondition(isEnnemyInLineCondition);



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
                && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f mouseFloatPosition = window.mapPixelToCoords(mousePosition);
                int line = static_cast<int>((mouseFloatPosition.y / window.getSize().y) * 4.f);

                allEnnemies.push_back(new Ennemy(sf::Vector2f(mouseFloatPosition.x, 50 + line * (window.getSize().y - 150.f) / 3.f)));
            }
        }

        // Mise à jour des plantes et leurs projectiles
        for (auto& plant : allPlants) {
            plant->Update();
            behaviour->Update(plant);

            for (auto it = plant->balls.begin(); it != plant->balls.end(); ) {
                Projectile* ball = *it;
                ball->Update();

                bool ballDestroyed = false;

                // Vérifier la collision avec chaque ennemi
                for (auto enemyIt = allEnnemies.begin(); enemyIt != allEnnemies.end(); ) {
                    Ennemy* enemy = *enemyIt;

                    // Calcul de la distance entre la balle et l'ennemi
                    int test = enemy->getPosition().x;
                    int test2 = enemy->getPosition().y;
                    float dx = ball->x - enemy->getPosition().x;
                    float dy = ball->y - enemy->getPosition().y;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    // Si la distance est inférieure à la somme des rayons, il y a collision
                    if (distance < ball->mShape.getRadius() + enemy->ennemyShape.getRadius()) {
                        // Supprime la balle et l'ennemi
                        it = plant->balls.erase(it);
                        enemyIt = allEnnemies.erase(enemyIt);
                        ballDestroyed = true;
                        delete ball;
                        delete enemy;
                        break;
                    }
                    else {
                        ++enemyIt;
                    }
                }

                if (!ballDestroyed) {
                    ++it;
                }
            }
        }

        // Mise à jour de la position des ennemis restants
        for (auto& enemy : allEnnemies) {
            enemy->Update();
        }

        // Affichage
        window.clear();

        // Afficher les plantes et leurs projectiles
        for (auto& plant : allPlants) {
            window.draw(plant->plantShape);
            window.draw(plant->textAmmo);

            for (auto projectile : plant->balls) {
                projectile->Draw(window);
            }
        }

        // Afficher les ennemis
        for (auto& enemy : allEnnemies) {
            window.draw(enemy->ennemyShape);
        }

        window.display();
    }


    for (int i = 0; i < 4; i++) {
        delete allPlants[i];
    }
    delete behaviour;

    return 0;
}