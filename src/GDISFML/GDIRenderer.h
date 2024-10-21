#pragma once

#ifdef GDI_MODE

#include "RenderInterface.h"
#include "GDIWindow.h"

class Texture;
struct DrawCommand;

#include "Surface.h"
#include "GDISurface.h"


class GDIRenderer : public RenderInterface
{
public:
    GDIRenderer();
    ~GDIRenderer();

    void Initialize() override {};

    // Clear render
    void PreDraw(Surface* surface) override;
    void DrawObject(DrawCommand& drawCommand) override;
    void RenderText(DrawCommand& drawCommand) override;
    void PostDraw() override;

private:
    GDISurface* m_surface;
};

#endif