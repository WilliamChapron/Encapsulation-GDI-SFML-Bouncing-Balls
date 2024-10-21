#pragma once

class Font {
public:
    Font();               
    virtual ~Font();

    virtual void Load(std::string& fontName) = 0;
};