#include "pch.h"
#include "Behaviour.hpp"
#include "Transition.hpp"
#include "Plant.hpp"
#include "Action.hpp"

Behaviour::Behaviour() {}

Behaviour::~Behaviour() {
    for (auto it : mTransitions) {
        for (int i = 0; i < it.second.size(); ++i) {
            delete it.second[i];
        }
    }

    for (auto it : mActions) {
        for (int i = 0; i < it.second.size(); ++i) {
            delete it.second[i];
        }
    }
}

void Behaviour::AddTransition(Context::State key, Transition* value) {
    mTransitions[key].push_back(value);
}

void Behaviour::AddAction(Context::State key, Action* value) {
    mActions[key].push_back(value);
}

void Behaviour::Start(Plant* plant) {
    for (const auto& a : mActions[plant->getState()]) {
        a->Start(plant);
    }
}

void Behaviour::Update(Plant* plant) {
    for (const auto& a : mActions[plant->getState()]) {
        a->Update(plant);
    }

    for (const auto& a : mTransitions[plant->getState()]) {
        a->Try(plant);
    }
}

void Behaviour::End(Plant* plant) {
    for (const auto& a : mActions[plant->getState()]) {
        a->End(plant);
    }
}
