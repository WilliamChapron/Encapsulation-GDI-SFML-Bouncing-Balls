#pragma once

class Texture;
class Font;

// Shape or text types
enum class DrawType {
    Rectangle,
    Circle,
    Text
};

// Render object informations
struct DrawCommand {
    DrawType type;
    Texture* texture = nullptr;
    Text* text = nullptr;

    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    float radius = 0.0f;

    unsigned int color[3] = { 255, 255, 255 };
};
