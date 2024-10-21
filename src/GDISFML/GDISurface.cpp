#include "pch.h"

#ifdef GDI_MODE

#include "GDISurface.h"

GDISurface::GDISurface(HDC hdc, int width, int height) : m_hdcDisplay(hdc), m_width(width), m_height(height) {
    // Create DC & Bitmap for draw (double buffering), the res for display  are already create
    m_hdcDraw = CreateCompatibleDC(m_hdcDisplay);  
    m_hBitmap = CreateCompatibleBitmap(m_hdcDisplay, m_width, m_height);  
    SelectObject(m_hdcDraw, m_hBitmap);  // Link HDC to bitmap
}

GDISurface::~GDISurface() {
    DeleteObject(m_hBitmap);    
    DeleteDC(m_hdcDraw);        
}
#endif 