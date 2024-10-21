#pragma once


#ifdef GDI_MODE 

#include "Texture.h"      
#include "BitmapTexture.h" 

class GDITexture : public Texture {
public:
    GDITexture();
    ~GDITexture();

    void Load(const char* fileName, HDC hdc = nullptr) override;

    BitmapTexture* m_srcTexture; // BMP File load
    HBITMAP m_DIBitmap; // Store texture info
    HDC m_destHDC; // HDC for store image temporarily
};

#endif
