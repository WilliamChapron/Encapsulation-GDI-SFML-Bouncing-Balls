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
        mShape.setOrigin(mShape.getRadius(), mShape.getRadius());
        mShape.setFillColor(sf::Color::White);
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

    sf::CircleShape plantShape;


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
        plantShape.setFillColor(sf::Color::Blue);
        plantShape.setRadius(20);
    }

    ~Plant() {}

    void setState(Context::State state) {
        mState = state;
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

    int getAmmoCount() const {
        return mAmmoCount;
    }

    void refillMagazine() {
        mAmmoCount = mMaxAmmo;
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
        textAmmo.setFillColor(sf::Color::White);
    }

    void setTextNbAmmo() {
        textAmmo.setString(std::to_string(mMaxAmmo + mAmmoCount));
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
