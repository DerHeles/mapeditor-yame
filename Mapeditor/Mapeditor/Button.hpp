#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button : public sf::Transformable, public sf::Drawable
{
public:
	enum ButtonAction
	{
		NEW, SAVE, LOAD, CONFIG, NO_ACTION
	};

	Button(float x, float y, const std::string& texturePath, ButtonAction action);
	void handleMouseMovement(float x, float y);
	void handleMouseClick(float x, float y, int button);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	ButtonAction m_action;
};

#endif