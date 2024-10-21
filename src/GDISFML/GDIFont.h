#pragma once

#ifdef GDI_MODE

#include "Font.h"

class GDIFont : public Font {
public:
    GDIFont();
    ~GDIFont();    

    void Load(std::string& fontName) override;

    HFONT GetHFont() const { return m_hFont;}
private:
    HFONT m_hFont; // GDI font object
};

#endif