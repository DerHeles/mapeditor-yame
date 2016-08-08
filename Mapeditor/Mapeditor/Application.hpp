#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include "GUI.hpp"

class Application
{
public:
	Application(unsigned int width, unsigned int height);
	~Application();
	void run();

	void changeCurrentPlacingTileValue(int tilePlaceValue);

private:
	void update(sf::Time elapsedTime);
	void processEvents();
	void render();

	void handleMouseMove(const sf::Event& event);
	void handleMouseButtonPress(const sf::Event& event);
	void handleMouseButtonRelease(const sf::Event& event);
	void handleMouseWheelScroll(const sf::Event& event);
	void handleKey(const sf::Event& event);

	bool isAboveMapArea(int x, int y) const;
	bool isAboveGUI(int x, int y) const;

	void extractTilesFromTileset(const std::string& tileset, sf::Vector2u tileSize);

	sf::RenderWindow m_window;
	TileMap m_map;
	sf::View m_view;

	int m_currentTilePlacingValue;
	//sf::RectangleShape m_cursorShape; über GUI geregelt

	//GUI
	GUI m_gui;
	sf::View m_guiView;

	sf::Vector2f m_dragStart;
	sf::Vector2f m_dragEnd;
	bool m_dragging; //um sicherzustellen, dass vor dem Auslösen von released auch innerhalb des fenster gepressed worden ist
};

#endif