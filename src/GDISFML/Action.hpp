#pragma once

class Plant;

class Action {
public:
    virtual void Start(Plant* plant) = 0;
    virtual void Update(Plant* plant) = 0;
    virtual void End(Plant* plant) = 0;
};
