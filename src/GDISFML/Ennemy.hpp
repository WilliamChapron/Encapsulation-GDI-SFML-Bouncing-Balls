#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Context.hpp"
#include "Behaviour.hpp"
#include <iostream>
#include <string>

class Ennemy {
private:

public:
	sf::CircleShape ennemyShape;
	std::string mName;

	Ennemy() :  mName("Default")
	{
	}

	Ennemy(sf::Vector2f position)
		: mName("Zombie")
	{
		ennemyShape.setRadius(15.f);
		ennemyShape.setOrigin(ennemyShape.getRadius(), ennemyShape.getRadius());
		ennemyShape.setPosition(position);
		ennemyShape.setFillColor(sf::Color::Red);
	}

	sf::Vector2f getPosition() {
		return ennemyShape.getPosition();
	}

	void Update() {
		sf::Vector2f pos = ennemyShape.getPosition();
		ennemyShape.setPosition(pos.x - 0.03, pos.y);
	}

	void setPosition(sf::Vector2f position)
	{
		ennemyShape.setPosition(position);
	}

};