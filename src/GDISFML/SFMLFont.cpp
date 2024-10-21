#include "pch.h"

#ifdef SFML_MODE

#include "SFMLFont.h" 

SFMLFont::SFMLFont() {
}

SFMLFont::~SFMLFont() {
}

void SFMLFont::Load(std::string& fontName) {
    if (!m_font.loadFromFile(fontName)) {
        std::cerr << "Failed to load font: " << fontName << std::endl;
    }
}

#endif
