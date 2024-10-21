#include "pch.h"

#include "SFMLRenderer.h" 
#include "SFMLTexture.h" 
#include "SFMLFont.h" 
#include "SFMLSurface.h" 

#include "Text.h" 


#include "./CommonStructures.h" 

#ifdef SFML_MODE

SFMLRenderer::SFMLRenderer() {
}

void SFMLRenderer::PreDraw(Surface* surface) {
    m_surface = static_cast<SFMLSurface*>(surface);
    m_surface->GetRenderTarget()->clear(sf::Color::Black);

}

void SFMLRenderer::DrawObject(DrawCommand& drawCommand) {
    switch (drawCommand.type) {
        case DrawType::Rectangle: {
                SFMLTexture* sfTexture = static_cast<SFMLTexture*>(drawCommand.texture);
                
                sf::RectangleShape rectangle(sf::Vector2f(150.f, 100.f));
                if (sfTexture) {
                    rectangle.setTexture(sfTexture->m_texture);
                }
                else {
                    rectangle.setFillColor(sf::Color::Red);
                }
                rectangle.setPosition(drawCommand.x, drawCommand.y);


                m_surface->GetRenderTarget()->draw(rectangle);

                break;

        }
        case DrawType::Circle: {
            SFMLTexture* sfTexture = static_cast<SFMLTexture*>(drawCommand.texture);
            sf::CircleShape circle(drawCommand.radius); 
            circle.setPosition(drawCommand.x - drawCommand.radius, drawCommand.y - drawCommand.radius); 
            if (sfTexture) {
                circle.setTexture(sfTexture->m_texture);
            } else {
                circle.setFillColor(sf::Color::Red);
            }
            m_surface->GetRenderTarget()->draw(circle);
            break;
        }
    }
    
}

void SFMLRenderer::RenderText(DrawCommand& drawCommand) {
    sf::Text& text = static_cast<SFMLText*>(drawCommand.text)->GetText();

    text.setPosition(drawCommand.x, drawCommand.y);

    unsigned int red = drawCommand.color[0];
    unsigned int green = drawCommand.color[1];
    unsigned int blue = drawCommand.color[2];
    text.setFillColor(sf::Color(red, green, blue));

    m_surface->GetRenderTarget()->draw(text);
}

void SFMLRenderer::PostDraw() {
    m_surface->GetRenderTarget()->display();
}

#endif