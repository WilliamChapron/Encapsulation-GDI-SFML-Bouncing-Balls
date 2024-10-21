#pragma once

#ifdef GDI_MODE

#include "Surface.h"

class GDISurface : public Surface {
public:
    GDISurface(HDC hdc, int width, int height);
    ~GDISurface();

    inline HDC GetHDC() const { return m_hdcDisplay; }

    inline HDC GetDrawingHDC() const { return m_hdcDraw; }
    inline void SetDrawingHDC(HDC hdc) { m_hdcDraw = hdc; }

    inline HBITMAP GetDrawingHBitmap() const { return m_hBitmap; }
    inline void SetDrawingHBitmap(HBITMAP hBitmap) { m_hBitmap = hBitmap; }

    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }

private:
    HDC m_hdcDisplay;  // HDC display
    HDC m_hdcDraw;     // HDC offscreen for double buffering
    HBITMAP m_hBitmap; // Bitmap -> store the drawing result
    int m_width;
    int m_height;
};

#endif