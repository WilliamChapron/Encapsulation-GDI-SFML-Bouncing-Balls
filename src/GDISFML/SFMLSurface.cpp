#include "pch.h"

#include "SFMLSurface.h"

#ifdef SFML_MODE

SFMLSurface::SFMLSurface(sf::RenderWindow* surface) : m_surface(surface) {

}

SFMLSurface::~SFMLSurface() {
}

#endif