#pragma once

class Texture {
public:
    Texture();
    virtual ~Texture() {};

    virtual void Load(const char* fileName, HDC hdc = nullptr) = 0;
};