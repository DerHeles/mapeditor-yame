#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class GUI;

class Button : public sf::Transformable, public sf::Drawable
{
public:
	enum class ButtonAction
	{
		NEW, SAVE, LOAD, CONFIG, TILE_VIEW, COLLISION_VIEW, ARROW_LEFT, ARROW_RIGHT, EMPTY_TILE, DELETE, PLACE, NO_ACTION
	};

	Button(float x, float y, ButtonAction action, GUI *gui);
	void handleMouseMove(float x, float y);
	void handleMouseClick(float x, float y, int button);
	void reset();
	void update(sf::Time elapsedTime);
	void activate();
	void deactivate();
	bool isActive();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Texture m_activeTexture;
	sf::Texture m_inactiveTexture;
	sf::Sprite m_sprite;

	ButtonAction m_action;
	sf::Time m_clickedTime;
	bool m_clicked;
	bool m_mouseOver;
	bool m_active;

	GUI *m_gui;
};

#endif