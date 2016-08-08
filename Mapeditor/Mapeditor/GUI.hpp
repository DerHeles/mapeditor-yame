#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "TileButton.hpp"

class Application;

class GUI : public sf::Transformable, public sf::Drawable
{
public:
	enum Mode
	{
		TILE, COLLSION
	};

	GUI(unsigned width, unsigned height, Application *application);
	~GUI();
	void handleMouseMove(float x, float y);
	void handleMouseClick(float x, float y, int button);
	void resize(unsigned int width, unsigned int height);
	void resetButtons();
	void update(sf::Time elapsedTime);
	void changeToMode(Mode mode);
	void changeCurrentPlacingTile(int tilePlaceValue);

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape m_borderShapes[3];

	std::vector<Button*> m_buttons;
	std::vector<Button*> m_tileLayerButtons;
	std::vector<Button*> m_collisionLayerButtons;
	std::vector<TileButton*> m_tileButtons;

	sf::Sprite m_layerSprite;
	sf::Texture m_layerTexture;

	Mode m_mode;
	sf::RectangleShape m_tilesBackground;

	sf::Texture m_tileset;

	//Tilebuttons
	sf::Texture m_noTileTexture;

	Application *m_application;

	sf::RectangleShape m_currentPlacingTileLabel;
};

#endif