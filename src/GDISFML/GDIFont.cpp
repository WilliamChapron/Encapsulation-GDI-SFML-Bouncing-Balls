#include "pch.h"

#ifdef GDI_MODE

#include "GDIFont.h"

GDIFont::GDIFont() {
}

GDIFont::~GDIFont() {
    DeleteObject(m_hFont);
}

void GDIFont::Load(std::string& fontName) {
    std::wstring wFontName(fontName.begin(), fontName.end());


    m_hFont = CreateFont(
        32,                
        0, 0, 0,           
        FW_NORMAL,         
        FALSE,             
        FALSE,             
        FALSE,             
        DEFAULT_CHARSET,   
        OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,   
        DEFAULT_PITCH | FF_DONTCARE, 
        L"Arial"
    );
}

#endif