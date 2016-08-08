#ifndef TILE_BUTTON_HPP
#define TILE_BUTTON_HPP

#include <SFML/Graphics.hpp>

class GUI;

class TileButton : public sf::Transformable, public sf::Drawable
{
public:
	enum ButtonAction
	{
		NEW, SAVE, LOAD, CONFIG, TILES, COLLISION, ARROW_LEFT, ARROW_RIGHT, EMPTY_TILE, DELETE, PLACE, NO_ACTION
	};

	TileButton(float x, float y, sf::Texture *startTexture, int tileID, GUI *gui);
	void handleMouseMove(float x, float y);
	void handleMouseClick(float x, float y, int button);
	void reset();
	void update(sf::Time elapsedTime);
	void setTexture(sf::Texture *texture);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape m_shape;

	ButtonAction m_action;
	sf::Time m_clickedTime;
	bool m_clicked;
	bool m_mouseOver;
	bool m_active;

	GUI *m_gui;
	int m_currentTileID;
};

#endif