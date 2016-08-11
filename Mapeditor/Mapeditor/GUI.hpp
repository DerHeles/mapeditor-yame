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

	enum class ButtonID
	{
		NEW, LOAD, CONFIG, SAVE, TILE_VIEW, COLLISION_VIEW, PLACE, DELETE, ARROW_LEFT, ARROW_RIGHT
	};

	GUI(unsigned int width, unsigned int height, Application *application);
	~GUI();
	void handleMouseMove(float x, float y);
	void handleMouseClick(float x, float y, int button);
	void resize(unsigned int width, unsigned int height);
	void resetButtons();
	void update(sf::Time elapsedTime);
	void changeToMode(Mode mode);
	void changeCurrentPlacingTile(int tilePlaceValue, sf::IntRect textureRect);
	void loadTiles(const std::string& tileset, float tile_width, float tile_height);

	void nextPage();
	void previousPage();

	static int index(ButtonID id);

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateTileButtons();

	sf::RectangleShape m_borderShapes[6];

	std::vector<Button*> m_buttons;
	std::vector<TileButton*> m_tileButtons;

	Mode m_mode;
	sf::RectangleShape m_tilesBackground;

	sf::Texture m_tileset;
	sf::Vector2u m_tileSize; //tiles werden immer als 50x50 im GUI angezeigt, haben aber variable größe in der map

	//Tilebuttons
	sf::Texture m_noTileTexture;
	sf::Texture m_deactivatedTileTexture;

	Application *m_application;

	sf::RectangleShape m_currentPlacingTile;
	sf::Sprite m_currentPlacingTileFrame;
	sf::Sprite m_deactivedPlacingTile;
	sf::Texture m_frameTexture;

	int m_currentTilePage;
	int m_amountOfTilePages;
	int m_amountOfDifferentTiles;
};

#endif