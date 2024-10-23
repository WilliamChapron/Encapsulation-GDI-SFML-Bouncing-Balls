#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Context.hpp"
#include "Behaviour.hpp"
#include <iostream>

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
    }

    void Update() {
        if (mBehaviour) {
            mBehaviour->Update(this);
        }
    }
};
