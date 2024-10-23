#pragma once

#include "Context.hpp"
#include <map>
#include <vector>

class Action;
class Plant;
class Transition;

class Behaviour {
    std::map<Context::State, std::vector<Action*>> mActions;
    std::map<Context::State, std::vector<Transition*>> mTransitions;
public:
    Behaviour();
    ~Behaviour();

    void Start(Plant* plant);
    void Update(Plant* plant);
    void End(Plant* plant);

    void AddAction(Context::State key, Action* value);
    void AddTransition(Context::State key, Transition* value);
};
