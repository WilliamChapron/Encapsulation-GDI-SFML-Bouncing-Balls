#include "pch.h"

#ifdef GDI_MODE 

#include "GDITexture.h"

GDITexture::GDITexture() : m_srcTexture(nullptr), m_destHDC(nullptr), m_DIBitmap(nullptr) {}

GDITexture::~GDITexture() {
    if (m_srcTexture) {
        delete m_srcTexture;
        m_srcTexture = nullptr; 
    }
    if (m_DIBitmap) {
        DeleteObject(m_DIBitmap);
        m_DIBitmap = nullptr; 
    }
    if (m_destHDC) {
        DeleteDC(m_destHDC);
        m_destHDC = nullptr;
    }
}

void GDITexture::Load(const char* fileName, HDC hdc) {
    m_srcTexture = new BitmapTexture();

    if (m_srcTexture->LoadFile(fileName)) {
        m_DIBitmap = CreateDIBitmap(
            hdc,
            m_srcTexture->m_infoHeader,
            CBM_INIT,
            m_srcTexture->m_pixels,
            m_srcTexture->m_info,
            DIB_RGB_COLORS
        );

        if (!m_DIBitmap) {
            std::cerr << "Failed to create DIBitmap for texture: " << fileName << std::endl;
            delete m_srcTexture; 
            return;
        }

        m_destHDC = CreateCompatibleDC(hdc);
        if (!m_destHDC) {
            std::cerr << "Failed to create compatible DC" << std::endl;
            DeleteObject(m_DIBitmap); 
            delete m_srcTexture;
            return;
        }

        SelectObject(m_destHDC, m_DIBitmap);
    } else {
        std::cerr << "Failed to load texture file: " << fileName << std::endl;
        delete m_srcTexture; 
    }
}

#endif
