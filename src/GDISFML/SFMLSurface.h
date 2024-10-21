#pragma once

#ifdef SFML_MODE

#include "Surface.h"

class SFMLSurface : public Surface
{
public:
    SFMLSurface(sf::RenderWindow* surface);
    ~SFMLSurface();

    inline sf::RenderWindow* GetRenderTarget() const { return m_surface; }

private:
    sf::RenderWindow* m_surface;
};

#endif
