#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Context.hpp"
#include "Behaviour.hpp"
#include <iostream>
#include <string>

class Plant {
    sf::Vector2f mPosition;
    std::string mName;
    int mAmmoCount, mMaxAmmo;
    Context::State mState;
    Behaviour* mBehaviour;

public:
    Plant() : mPosition(0, 0), mName("Default"), mAmmoCount(0), mMaxAmmo(10), mState(Context::State::Shooting), mBehaviour(nullptr) {}

    Plant(sf::Vector2f position, Behaviour* plant_behaviour, int ammo_count)
        : mPosition(position), mName("Plant"), mAmmoCount(ammo_count), mMaxAmmo(10), mState(Context::State::Shooting), mBehaviour(plant_behaviour) {}

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
    }

    void Update() {
        setTextNbAmmo();
        if (mBehaviour) {
            mBehaviour->Update(this);
        }
    }
};
