#include "pch.h"

#ifdef SFML_MODE

#include "SFMLTexture.h"

SFMLTexture::SFMLTexture() {
    m_texture = new sf::Texture();
}

SFMLTexture::~SFMLTexture() {
    delete m_texture;
}

void SFMLTexture::Load(const char* fileName, HDC hdc) {
    m_texture->loadFromFile(fileName);
}

#endif