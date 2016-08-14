#ifndef TILE_BUTTON_HPP
#define TILE_BUTTON_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"

class GUI;

class TileButton : public sf::Transformable, public sf::Drawable
{
public:

	TileButton(float x, float y, sf::Texture *noTileTexture, sf::Texture *deactivatedTexture, int tileID, GUI *gui);
	void handleMouseClick(float x, float y, int button) const;

	void setTileset(sf::Texture *texture);
	void showTile();
	void showNoTile();
	void setTileTextureRect(const sf::IntRect& rect);
	void activate();
	void deactivate();
	bool isActive() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape m_shape;

	sf::Time m_clickedTime;
	bool m_clicked;
	bool m_mouseOver;
	bool m_active;

	GUI *m_gui;
	int m_currentTileID;

	sf::Texture *m_noTileTexture;
	sf::Texture *m_deactivatedTileTexture;
	sf::Texture *m_tileset;
	sf::IntRect m_texRect;
	bool m_showingTile;
};

#endif