#pragma once

class Text
{
public:
    Text() {};
    virtual ~Text() {};

    virtual void SetText(std::wstring& text) = 0;
};

#ifdef SFML_MODE

#include "SFMLFont.h"

class SFMLText : public Text
{
public:
    SFMLText(SFMLFont* font, std::wstring& text) {
        m_text.setFont(font->GetFont());
        m_text.setString(text);
    }

    ~SFMLText() {};

    inline void SetText(std::wstring& text) override {
        m_text.setString(text);
    }

    inline sf::Text& GetText() { return m_text; }

private:
    sf::Text m_text;
};

#endif

#ifdef GDI_MODE

class GDIFont;

class GDIText : public Text
{
public:
    GDIText(GDIFont* font, std::wstring& text) {
        m_text = text;
        m_font = font;
    }

    ~GDIText()
    {
    }

    virtual void SetText(std::wstring& text) override {
        m_text = text;
    }

    const std::wstring& GetText() const {
        return m_text;
    }

    GDIFont* GetFont() {
        return m_font;
    }

private:
    std::wstring m_text;
    GDIFont* m_font;
};

#endif
