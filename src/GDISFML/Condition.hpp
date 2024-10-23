#pragma once

class Plant;

class Condition {
public:
    virtual bool Test(Plant* plant) = 0;
};
