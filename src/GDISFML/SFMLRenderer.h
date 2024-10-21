#pragma once

#ifdef SFML_MODE

#include "RenderInterface.h"
#include "SFMLWindow.h"

class SFMLSurface;

class SFMLRenderer : public RenderInterface
{
public:
    SFMLRenderer();
    ~SFMLRenderer() {};

    void Initialize() override {};
    void PreDraw(Surface* surface) override;
    void DrawObject(DrawCommand& drawCommand) override;
    void RenderText(DrawCommand& drawCommand) override;
    void PostDraw() override;

private:
    SFMLSurface* m_surface;
};

#endif
