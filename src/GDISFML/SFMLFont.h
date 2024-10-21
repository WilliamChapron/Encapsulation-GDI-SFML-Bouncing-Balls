#pragma once

#ifdef SFML_MODE

#include "Font.h" 

#include <SFML/Graphics.hpp>
#include <string>

class SFMLFont : public Font {
public:
    SFMLFont();
    ~SFMLFont();

    void Load(std::string& fontName) override;

    sf::Font& GetFont() {
        return m_font; 
    }
private:
    sf::Font m_font; 
};

#endif
