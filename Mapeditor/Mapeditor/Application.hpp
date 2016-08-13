#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include "EditorTileMap.hpp"
#include "GUI.hpp"

class Application
{
public:
	Application(unsigned int width, unsigned int height);
	~Application();
	void run();

	void changeCurrentPlacingTileValue(int tilePlaceValue);
	void changeToMode(GUI::Mode mode);
	void changeCurrentCollisionTileValue(bool collision);
	void createMap();
	void loadMap();
	void configMap();
	void saveMap();

private:
	void update(sf::Time elapsedTime);
	void processEvents();
	void render();

	void setIcon();

	void handleMouseMove(const sf::Event& event);
	void handleMouseButtonPress(const sf::Event& event);
	void handleMouseButtonRelease(const sf::Event& event);
	void handleMouseWheelScroll(const sf::Event& event);
	void handleKey(const sf::Event& event);

	bool isAboveMapArea(int x, int y) const;
	bool isAboveGUI(int x, int y) const;
	bool aboveValidMapArea(float x, float y) const;

	void extractTilesFromTileset(const std::string& tileset, sf::Vector2u tileSize);

	sf::RenderWindow m_window;
	EditorTileMap m_map;
	sf::View m_view;

	int m_currentTilePlacingValue;
	sf::RectangleShape m_selectedTilesShape;
	sf::Vector2u m_tileSize;
	bool m_showSelectedTile;
	sf::Vector2u m_mapSize;

	//GUI
	GUI m_gui;
	sf::View m_guiView;

	sf::Vector2f m_dragStart;
	sf::Vector2f m_dragEnd;
	bool m_dragging; //um sicherzustellen, dass vor dem Auslösen von released auch innerhalb des fenster gepressed worden ist

	GUI::Mode m_mode;
	bool m_collisionLayerPlaceCollisions;
};

#endif