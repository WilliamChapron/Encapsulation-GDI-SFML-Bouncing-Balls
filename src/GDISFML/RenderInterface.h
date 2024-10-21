#pragma once

class Texture;
 
struct DrawCommand;

class Surface;

class RenderInterface {
public:
    RenderInterface() {}
    virtual ~RenderInterface() {};

    virtual void Initialize() {};
    // Clear last render 
    virtual void PreDraw(Surface* surface) {};
    // Draw shape
    virtual void DrawObject(DrawCommand& drawCommand) {};
    // Draw text object that contain font automatically
    virtual void RenderText(DrawCommand& drawCommand) {};
    // Diplay final render (double buffering)
    virtual void PostDraw() {};
};




