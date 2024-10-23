#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Context.hpp"
#include "Behaviour.hpp"
#include <iostream>
#include <string>

class Timer {
public:
    bool isInit = false;

    void Start() {
        isInit = true;
        startTime = std::chrono::steady_clock::now();
    }

    bool CheckEndTimer(double seconds) {
        currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        return elapsed.count() >= seconds;
    }

private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point currentTime;
};


class Projectile {
public:
    float x, y;                 
    float mVelocityX, mVelocityY; 
    float mDamage;
    sf::CircleShape mShape;

    int ballCounter = 0;


    Projectile(float posX, float posY, float velocityX, float velocityY, float damage)
        : x(posX), y(posY), mVelocityX(velocityX), mVelocityY(velocityY), mDamage(damage) {
        mShape.setRadius(5.0f);
        mShape.setFillColor(sf::Color::Red);
        mShape.setPosition(x, y); 
    }

    void Update() {
        //std::cout << "Update ball\n" << ": " << ballCounter;
        x += mVelocityX;
        mShape.setPosition(x, y); 
        ballCounter++;
    }

    void Draw(sf::RenderWindow& window) {
        window.draw(mShape);
    }

    float GetDamage() const {
        return mDamage;
    }
};


class Plant {
public:
    // Rpm
    bool isShootPerformed = false;
    Timer rpmTimer;
    // Ammo
    Timer reloadingTimer;

    sf::RectangleShape plantShape;


    sf::Vector2f mPosition;
    std::string mName;
    int mAmmoCount = 10, mMaxAmmo = 10;
    Context::State mState;
    Behaviour* mBehaviour;

    std::string StateToString() {
        switch (mState) {
        case Context::State::CanShoot:   return "CanShoot";
        case Context::State::Reloading:  return "Reloading";
        case Context::State::Dead:       return "Dead";
        default:                         return "Unknown State";
        }
    }

    std::vector<Projectile*> balls;


    Plant() : mPosition(0, 0), mName("Default") ,mState(Context::State::Idle), mBehaviour(nullptr) 
    {
    }

    Plant(sf::Vector2f position, Behaviour* plant_behaviour, int ammo_count)
        : mPosition(position), mName("Plant"), mState(Context::State::Idle), mBehaviour(plant_behaviour)
    {
        plantShape.setPosition(position);
        plantShape.setFillColor(sf::Color::Green);
        plantShape.setSize(sf::Vector2f(40, 40));
    }

    ~Plant() {}

    void setState(Context::State state) {
        mState = state;
        return;

        switch (mState) {
        case Context::State::Idle:
            std::cout << "Idle state" << std::endl;
            break;
        case Context::State::CanShoot:
            std::cout << "CanShoot state" << std::endl;
            break;
        case Context::State::Reloading:
            std::cout << "Reloading state" << std::endl;
            break;
        case Context::State::Dead:
            std::cout << "Dead state" << std::endl;
            break;
        default:
            std::cout << "Unknown state" << std::endl;
            break;
        }
        std::cout << "\n";
    }

    Context::State getState() const {
        return mState;
    }

    sf::Color getColor() const {
        return sf::Color::Blue;
    }

    sf::Vector2f getPosition() const {
        return mPosition;
    }

    sf::Text textAmmo;
    sf::Font mfont;

    void loadTextFont() {
        if (!mfont.loadFromFile("C:/Users/benjaminbenon/Documents/GitHub/EncapsulationGDI-SFML/src/Hack-Regular.ttf"))
        {
            return;
        }
        textAmmo.setFont(mfont);
        textAmmo.setCharacterSize(20);
        textAmmo.setFillColor(sf::Color::Red);
    }

    void setTextNbAmmo() {
        textAmmo.setString(std::to_string(mAmmoCount));
        textAmmo.setPosition(mPosition);
    }

    bool shoot() {
        if (mAmmoCount > 0) {
            mAmmoCount--;
            return true;
        }
        return false;
        std::cout << "Shoot";
    }

    void Update() {
        setTextNbAmmo();
        if (mBehaviour) {
            mBehaviour->Update(this);
        }
    }
};
