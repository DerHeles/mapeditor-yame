#include "Button.hpp"
#include <iostream>


Button::Button(float x, float y, const std::string& texturePath, ButtonAction action)
	:

	m_action(action)
{
	setPosition(x, y);

	if (!m_texture.loadFromFile(texturePath))
		std::cerr << "Button could not load texture!" << std::endl;

	m_sprite.setTexture(m_texture);
}

void Button::handleMouseMovement(float x, float y)
{
}

void Button::handleMouseClick(float x, float y, int button)
{
	sf::Vector2f v = getTransform().getInverse().transformPoint(x, y);

	if (m_sprite.getGlobalBounds().contains(v))
	{
		switch (m_action)
		{
		case NEW:
			std::cout << "NEW" << std::endl;
			break;
		case LOAD:
			std::cout << "LOAD" << std::endl;
			break;
		case SAVE:
			std::cout << "SAVE" << std::endl;
			break;
		case CONFIG:
			std::cout << "CONFIG" << std::endl;
			break;
		default:
			std::cout << "no action" << std::endl;
			break;
		}
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}
