#include "pch.h"



#ifdef GDI_MODE

#include "GDIRenderer.h"
#include "GDIWindow.h"
#include "GDITexture.h"
#include "GDIFont.h"
#include "GDISurface.h"

#include "Texture.h"
#include "App.h"
#include "Surface.h"
#include "Text.h"


GDIRenderer::GDIRenderer() {
    

}

GDIRenderer::~GDIRenderer() {
}

void GDIRenderer::PreDraw(Surface* surface) {
    m_surface = static_cast<GDISurface*>(surface);

    RECT rect;
    GetClipBox(m_surface->GetDrawingHDC(), &rect);  // Draw on the memory HDC
    HBRUSH brush = CreateSolidBrush(RGB(10, 55, 255));  // Set background color
    FillRect(m_surface->GetDrawingHDC(), &rect, brush);  // Fill the memory HDC
    DeleteObject(brush);
}


// #TODO DrawShape
void GDIRenderer::DrawObject(DrawCommand& drawCommand) {

    switch (drawCommand.type) {
        case DrawType::Rectangle: {

            if (drawCommand.texture != nullptr) {
                GDITexture* gdiTexture = static_cast<GDITexture*>(drawCommand.texture);
                StretchBlt(
                    m_surface->GetDrawingHDC(),  // Draw on the memory HDC
                    drawCommand.x,
                    drawCommand.y,
                    100,
                    100,
                    gdiTexture->m_destHDC,
                    0,
                    0,
                    gdiTexture->m_srcTexture->m_infoHeader->biWidth,
                    gdiTexture->m_srcTexture->m_infoHeader->biHeight,
                    SRCCOPY
                );
            }
            else {
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                HGDIOBJ oldBrush = SelectObject(m_surface->GetDrawingHDC(), hBrush);

                Rectangle(m_surface->GetDrawingHDC(), drawCommand.x, drawCommand.y, drawCommand.x + 100, drawCommand.y + 100);

                SelectObject(m_surface->GetDrawingHDC(), oldBrush);
                DeleteObject(hBrush);
            }
            

            break;

        }
        case DrawType::Circle: {
            const int radius = drawCommand.radius;
            const int diameter = radius * 2;

            HRGN hRgn = CreateEllipticRgn(
                drawCommand.x - radius,
                drawCommand.y - radius,
                drawCommand.x + radius,
                drawCommand.y + radius
            );

            SelectClipRgn(m_surface->GetDrawingHDC(), hRgn);

            if (drawCommand.texture != nullptr) {
                GDITexture* gdiTexture = static_cast<GDITexture*>(drawCommand.texture);

                BLENDFUNCTION blendFunc;
                blendFunc.BlendOp = AC_SRC_OVER; 
                blendFunc.BlendFlags = 0;
                blendFunc.SourceConstantAlpha = 255; 
                blendFunc.AlphaFormat = AC_SRC_ALPHA; 

                AlphaBlend(
                    m_surface->GetDrawingHDC(), 
                    drawCommand.x - radius, 
                    drawCommand.y - radius, 
                    diameter, 
                    diameter, 
                    gdiTexture->m_destHDC, 
                    0, 
                    0, 
                    gdiTexture->m_srcTexture->m_infoHeader->biWidth, 
                    gdiTexture->m_srcTexture->m_infoHeader->biHeight, 
                    blendFunc 
                );
            }
            else {
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                HGDIOBJ oldBrush = SelectObject(m_surface->GetDrawingHDC(), hBrush);
                Ellipse(m_surface->GetDrawingHDC(), drawCommand.x - radius, drawCommand.y - radius, drawCommand.x + radius, drawCommand.y + radius);
                SelectObject(m_surface->GetDrawingHDC(), oldBrush);
                DeleteObject(hBrush);
            }

            SelectClipRgn(m_surface->GetDrawingHDC(), NULL);  
            DeleteObject(hRgn);  

            break;
        }
    }
}

void GDIRenderer::RenderText(DrawCommand& drawCommand) {
    GDIText* gdiText = static_cast<GDIText*>(drawCommand.text);
    HFONT font = gdiText->GetFont()->GetHFont();

    SetBkMode(m_surface->GetDrawingHDC(), TRANSPARENT);

    HGDIOBJ oldFont = SelectObject(m_surface->GetDrawingHDC(), font);


    COLORREF color = RGB(drawCommand.color[2], drawCommand.color[1], drawCommand.color[0]); // Utilise l'ordre BGR
    SetTextColor(m_surface->GetDrawingHDC(), color);

    TextOut(m_surface->GetDrawingHDC(),
        static_cast<int>(drawCommand.x),
        static_cast<int>(drawCommand.y),
        gdiText->GetText().c_str(),
        static_cast<int>(gdiText->GetText().length()));

    SelectObject(m_surface->GetDrawingHDC(), oldFont);
}

void GDIRenderer::PostDraw() {
    BitBlt(
        m_surface->GetHDC(),         
        0, 0,                        
        m_surface->GetWidth(),       
        m_surface->GetHeight(),      
        m_surface->GetDrawingHDC(),  
        0, 0,                        
        SRCCOPY                      
    );
}

#endif