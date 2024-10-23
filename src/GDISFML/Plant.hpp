#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Context.hpp"
#include "Behaviour.hpp"
#include <iostream>

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

    sf::RectangleShape plantShape;


    sf::Vector2f mPosition;
    std::string mName;
    int mAmmoCount, mMaxAmmo;
    Context::State mState;
    Behaviour* mBehaviour;

    std::vector<Projectile*> balls;


    Plant() : mPosition(0, 0), mName("Default"), mAmmoCount(0), mMaxAmmo(10), mState(Context::State::CanShoot), mBehaviour(nullptr) 
    {
    }

    Plant(sf::Vector2f position, Behaviour* plant_behaviour, int ammo_count)
        : mPosition(position), mName("Plant"), mAmmoCount(ammo_count), mMaxAmmo(10), mState(Context::State::CanShoot), mBehaviour(plant_behaviour)
    {
        plantShape.setPosition(position);
        plantShape.setFillColor(sf::Color::Green);
        plantShape.setSize(sf::Vector2f(40, 40));
    }

    ~Plant() {}

    void setState(Context::State state) {
        mState = state;
    }

    Context::State getState() const {
        return mState;
    }

    sf::Color getColor() const {
        return sf::Color::Green;
    }

    sf::Vector2f getPosition() const {
        return mPosition;
    }

    int getAmmoCount() const {
        return mAmmoCount;
    }

    void refillMagazine() {
        mAmmoCount = mMaxAmmo;
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
        if (mBehaviour) {
            mBehaviour->Update(this);
        }
    }
};
