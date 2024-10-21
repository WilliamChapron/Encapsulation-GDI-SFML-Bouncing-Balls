#pragma once

#ifdef SFML_MODE

#include "Texture.h"

class SFMLTexture : public Texture
{
public:
    SFMLTexture();
    ~SFMLTexture();

    void Load(const char* fileName, HDC hdc = nullptr) override;

    sf::Texture* m_texture;
};

#endif
