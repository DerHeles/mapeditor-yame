#include "Application.hpp"
#include "Configuration.hpp"
#include <iostream>
#include <fstream>

Application::Application(unsigned int width, unsigned int height)
	:
	m_window(sf::VideoMode(width, height), "YAME - Mode: TILE", sf::Style::Default),
	m_currentTilePlacingValue(-1),
	m_showSelectedTile(false),
	m_gui(width, height, this),
	m_dragging(false),
	m_mode(GUI::Mode::TILE),
	m_collisionLayerPlaceCollisions(true)
{
	m_window.setFramerateLimit(60);

	m_view.setSize(m_window.getSize().x - cfg::gui_width, m_window.getSize().y);
	m_view.setViewport(sf::FloatRect(1.f - (static_cast<float>(width) - cfg::gui_width) / static_cast<float>(width), 0.f, (static_cast<float>(width) - cfg::gui_width) / static_cast<float>(width), 1.f));

	m_guiView = m_window.getView();

	m_selectedTilesShape.setFillColor(sf::Color(255, 100, 100, 180));

	setIcon();
}

Application::~Application()
{

}

void Application::run()
{
	sf::Clock clock;
	while(m_window.isOpen())
	{
		processEvents();
		update(clock.restart());
		render();
	}
}

void Application::changeCurrentPlacingTileValue(int tilePlaceValue)
{
	m_currentTilePlacingValue = tilePlaceValue;
}

void Application::changeToMode(GUI::Mode mode)
{
	if(m_mode != mode)
	{
		m_mode = mode;
		if(mode == GUI::Mode::TILE)
		{
			m_window.setTitle("YAME - Mode: TILE");
			m_map.hideCollisionLayer();
		}
		else if(mode == GUI::Mode::COLLSION)
		{
			m_window.setTitle("YAME - Mode: COLLISION");
			m_map.showCollisionLayer();
		}
	}
}

void Application::changeCurrentCollisionTileValue(bool collision)
{
	m_collisionLayerPlaceCollisions = collision;
}

void Application::createMap()
{
	std::string tileset;
	int *tiles;
	int width;
	int height;
	int tile_width;
	int tile_height;
	int tile_gap;
	bool *collisionTiles;

	std::cout << "~~~Create Map~~~\ntileset name: " << std::flush;
	std::cin >> tileset;

	std::cout << "map width: " << std::flush;
	std::cin >> width;
	while (std::cin.fail()) {
		std::cout << "wrong input for map width!\nreenter map width: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> width;
	}

	std::cout << "map height: " << std::flush;
	std::cin >> height;
	while (std::cin.fail()) {
		std::cout << "wrong input for map height!\nreenter map height: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> height;
	}

	std::cout << "tile width: " << std::flush;
	std::cin >> tile_width;
	while (std::cin.fail()) {
		std::cout << "wrong input for tile width!\nreenter tile width: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> tile_width;
	}

	std::cout << "tile height: " << std::flush;
	std::cin >> tile_height;
	while (std::cin.fail()) {
		std::cout << "wrong input for tile height!\nreenter tile height: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> tile_height;
	}

	std::cout << "tile gap: " << std::flush;
	std::cin >> tile_gap;
	while (std::cin.fail()) {
		std::cout << "wrong input for tile gap!\nreenter tile gap: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> tile_gap;
	}

	tiles = new int[width * height];
	for (int i = 0; i < width * height; ++i)
		tiles[i] = 0;

	collisionTiles = new bool[width * height];
	for (int i = 0; i < width * height; ++i)
		collisionTiles[i] = false;

	if(m_map.load("tilesets/" + tileset, sf::Vector2u(tile_width, tile_height), tiles, width, height, tile_gap, collisionTiles))
	{
		std::cout << "Map created!" << std::endl;

		m_selectedTilesShape.setSize(sf::Vector2f(tile_width, tile_height));

		m_tileSize.x = tile_width;
		m_tileSize.y = tile_height;

		m_mapSize.x = width;
		m_mapSize.y = height;

		m_gui.mapLoaded();
		m_gui.loadTiles("tilesets/" + tileset, tile_width, tile_height, tile_gap);
	}
	else
	{
		std::cout << "Map creation failed!" << std::endl;
	}

	delete[] tiles;
	delete[] collisionTiles;
}

void Application::loadMap()
{
	std::string mapname;
	std::cout << "~~~Load Map~~~\nmap name (without .ymap!): " << std::flush;
	std::cin >> mapname;
	mapname.append(".ymap");

	std::ifstream mapfile(std::string("maps/") + mapname, std::ios::binary);

	if (mapfile.is_open())
	{
		m_map.loadFromFile(&mapfile);
		std::cout << "map loaded from \"" << "maps/" << mapname << "\"" << std::endl;

		m_tileSize.x = m_map.getTileSize().x;
		m_tileSize.y = m_map.getTileSize().y;

		m_mapSize.x = m_map.getMapSize().x;
		m_mapSize.y = m_map.getMapSize().y;

		m_selectedTilesShape.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));

		m_gui.loadTiles(m_map.getTexturePath(), m_tileSize.x, m_tileSize.y, m_map.getTileGap());
		m_gui.mapLoaded();
	}
	else
	{
		std::cerr << "error loading map from \"" << "maps/" << mapname << "\"" << std::endl;
	}
}

void Application::configMap()
{
	std::string tileset;
	int tile_width;
	int tile_height;
	int tile_gap;

	std::string currentTileset = m_map.getTexturePath();
	currentTileset.erase(0, 9); //remove "tilesets/"

	std::cout << "~~~Config existing Map~~~\ntileset name (currently = " << currentTileset << " ): "<< std::flush;
	std::cin >> tileset;

	std::cout << "tile width (currently = " << m_tileSize.x << " ): " << std::flush;
	std::cin >> tile_width;
	while (std::cin.fail()) {
		std::cout << "wrong input for tile width!\nreenter tile width: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> tile_width;
	}

	std::cout << "tile height (currently = " << m_tileSize.y << " ): " << std::flush;
	std::cin >> tile_height;
	while (std::cin.fail()) {
		std::cout << "wrong input for tile height!\nreenter tile height: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> tile_height;
	}

	std::cout << "tile gap (currently = " << m_map.getTileGap() << " ): " << std::flush;
	std::cin >> tile_gap;
	while (std::cin.fail()) {
		std::cout << "wrong input for tile gap!\nreenter tile gap: " << std::flush;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> tile_gap;
	}

	if (m_map.reload("tilesets/" + tileset, sf::Vector2u(tile_width, tile_height),  tile_gap))
	{
		std::cout << "Map configured!" << std::endl;

		m_selectedTilesShape.setSize(sf::Vector2f(tile_width, tile_height));

		m_tileSize.x = tile_width;
		m_tileSize.y = tile_height;

		m_gui.mapLoaded();
		m_gui.loadTiles("tilesets/" + tileset, tile_width, tile_height, tile_gap);
	}
	else
	{
		std::cout << "Map config failed!" << std::endl;
	}
}

void Application::saveMap() const
{

	std::string mapname;
	std::cout << "~~~Load Map~~~\nmap name (without .ymap!): " << std::flush;
	std::cin >> mapname;
	mapname.append(".ymap");

	std::ofstream mapfile(std::string("maps/") + mapname, std::ios::binary);

	if(mapfile.is_open())
	{
		m_map.saveToFile(&mapfile);
		std::cout << "map saved to \"" << mapname << "\"" << std::endl;
	}
	else
	{
		std::cerr << "error saving map to \"" << mapname << "\"" << std::endl;
	}
}

void Application::update(sf::Time elapsedTime)
{
	m_gui.update(elapsedTime);
}

void Application::processEvents()
{
	sf::Event event;
	while(m_window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::Closed:
				m_window.close();
				break;
			case sf::Event::MouseMoved:
				handleMouseMove(event);
				break;
			case sf::Event::MouseButtonPressed:
				handleMouseButtonPress(event);
				break;
			case sf::Event::MouseButtonReleased:
				handleMouseButtonRelease(event);
				break;
			case sf::Event::MouseWheelScrolled:
				handleMouseWheelScroll(event);
				break;
			case sf::Event::KeyPressed:
				handleKey(event);
				break;
			case sf::Event::Resized:
				if (event.size.width < 800)
				{
					if (event.size.height < cfg::gui_height)
						m_window.setSize(sf::Vector2u(800, cfg::gui_height));
					else
						m_window.setSize(sf::Vector2u(800, event.size.height));
				}
				if (event.size.height < cfg::gui_height)
					m_window.setSize(sf::Vector2u(event.size.width, cfg::gui_height));

				m_guiView.setSize(m_window.getSize().x, m_window.getSize().y);
				m_gui.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0,0), m_guiView));

				m_view.setViewport(sf::FloatRect(1.f - (static_cast<float>(m_window.getSize().x) - cfg::gui_width) / static_cast<float>(m_window.getSize().x), 0.f, (static_cast<float>(m_window.getSize().x) - cfg::gui_width) / static_cast<float>(m_window.getSize().x), 1.f));
				m_view.setSize(m_window.getSize().x - cfg::gui_width, m_window.getSize().y);

				break;
			default:
				;
		}
	}
}


void Application::handleMouseWheelScroll(const sf::Event& event)
{
	if (event.mouseWheelScroll.delta > 0)
		for (int i = 0; i < event.mouseWheelScroll.delta; ++i)
			m_view.zoom(0.95f);
	else if (event.mouseWheelScroll.delta < 0)
		for (int i = 0; i > event.mouseWheelScroll.delta; --i)
			m_view.zoom(1.05f);
}

void Application::handleKey(const sf::Event& event)
{
	switch (event.key.code)
	{
		case sf::Keyboard::Add:
			m_view.zoom(0.95f);
			break;
		case sf::Keyboard::Subtract:
			m_view.zoom(1.05f);
			break;
		case sf::Keyboard::Up:
		case sf::Keyboard::W:
			m_view.move(0, -cfg::MAP_SCROLL_OFFSET);
			break;
		case sf::Keyboard::Down:
		case sf::Keyboard::S:
			m_view.move(0, cfg::MAP_SCROLL_OFFSET);
			break;
		case sf::Keyboard::Left:
		case sf::Keyboard::A:
			m_view.move(-cfg::MAP_SCROLL_OFFSET, 0);
			break;
		case sf::Keyboard::Right:
		case sf::Keyboard::D:
			m_view.move(cfg::MAP_SCROLL_OFFSET, 0);
			break;
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		default:
			break;
	}
}

bool Application::isAboveMapArea(int x, int y) const
{
	return (cfg::gui_width < x && x < m_window.getSize().x && 0 < y && y < m_window.getSize().y);
}

bool Application::isAboveGUI(int x, int y) const
{
	return (0 < x && x <= cfg::gui_width && 0 < y && y < m_window.getSize().y);
}

bool Application::aboveValidMapArea(float x, float y) const
{
	if (x < 0 || x >= (m_mapSize.x * m_tileSize.x) || y < 0 || y >= (m_mapSize.y * m_tileSize.y))
		return false;
	return true;
}

void Application::render()
{
	m_window.clear(sf::Color(100, 100, 100));

	//TileMap
	m_window.setView(m_view);
	m_window.draw(m_map);
	if (m_showSelectedTile)
		m_window.draw(m_selectedTilesShape);

	//GUI
	m_window.setView(m_guiView);
	m_window.draw(m_gui);

	m_window.display();
}

void Application::setIcon()
{/* GIMP RGBA C-Source image dump (logo.c) */

	static const struct {
		unsigned int 	 width;
		unsigned int 	 height;
		unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
		unsigned char	 pixel_data[32 * 32 * 4 + 1];
	} ymap_icon = {
		32, 32, 4,
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000"
		"\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000"
		"\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377wx\177\377wx\177\377wx\177\377wx"
		"\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177"
		"\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377"
		"wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx"
		"\177\377wx\177\377wx\177\377wx\177\377wx\177\377\000\000\000\377\000\000\000\377wx\177"
		"\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263"
		"\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277"
		"\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263"
		"\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277"
		"\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263"
		"\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277"
		"\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277"
		"\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000"
		"\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\263\257\277\377wx\177\377"
		"\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377"
		"\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx"
		"\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263"
		"\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377"
		"\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377"
		"\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257"
		"\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\200\232\377\000\212\247\377\000\212\247\377\000\254\320\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\322\375\377\000\205\240"
		"\377\000{\225\377\000{\225\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177\377"
		"\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000O`\377\000\000\000\377"
		"\000\000\000\377\000\070D\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000k\201\377\000\000\000\377\000\000\000\377\000#+\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000"
		"\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277"
		"\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\313\365\377\000\000\000\377\000\000\000\377\000\000\000\377\000\247\311\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\322\375\377\000\011\013\377\000\000\000"
		"\377\000\000\000\377\000\250\313\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177"
		"\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"O`\377\000\000\000\377\000\000\000\377\000K[\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000l\203\377\000\000\000\377\000\000\000\377\000\065@\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257"
		"\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\313\365\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\000\261\325\377\000\324\377\377\000\322\375\377\000\020\024\377\000\000\000\377\000\000\000\377"
		"\000\255\321\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177"
		"\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000Se\377\000\000\000\377\000\000\000\377\000Ug\377\000\324\377\377\000v\216\377"
		"\000\000\000\377\000\000\000\377\000FU\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000"
		"\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277"
		"\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\313\365\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\271\337\377\000\016\021\377\000\000\000\377\000\000\000\377\000\261\325\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177\377"
		"\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000]p\377\000\000\000\377\000\000\000\377\000$,\377\000\000\000\377\000\000"
		"\000\377\000Rc\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000"
		"\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277"
		"\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\313\365\377\000\003"
		"\004\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\263\330\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177"
		"\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000g|\377\000\000\000\377\000\000\000\377\000\000"
		"\000\377\000Ug\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377"
		"\263\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\313\365\377\000\000\001\377\000\000\000\377\000\000\000\377\000\266\334\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257"
		"\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\011"
		"\013\377\000\000\000\377\000\000\000\377\000\312\363\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177\377\000\000"
		"\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\002\003\377\000\000\000\377\000\000\000"
		"\377\000\312\363\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177"
		"\377\263\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\000\000\377\000\000\000\377\000\000\000\377\000\312\363\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377"
		"\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377"
		"\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\000\000\377\000\000\000\377\000\000\000\377\000\312\363\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177"
		"\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\312\363\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377"
		"wx\177\377\263\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263"
		"\257\277\377\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377"
		"\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377"
		"\000\000\000\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324"
		"\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377"
		"\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377"
		"\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000"
		"\324\377\377\000\324\377\377\000\324\377\377\000\324\377\377\000\000\000\377\263\257"
		"\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377\263\257\277\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000"
		"\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000"
		"\377\000\000\000\377\000\000\000\377\000\000\000\377\263\257\277\377wx\177\377\000\000\000\377\000"
		"\000\000\377wx\177\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257"
		"\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377"
		"\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257"
		"\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377"
		"\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257"
		"\277\377\263\257\277\377\263\257\277\377\263\257\277\377\263\257\277\377"
		"\263\257\277\377\263\257\277\377wx\177\377\000\000\000\377\000\000\000\377wx\177\377"
		"wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx"
		"\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177"
		"\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377"
		"wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377wx\177\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000"
		"\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000"
		"\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377",
	};

	m_window.setIcon(ymap_icon.width, ymap_icon.height, ymap_icon.pixel_data);

}

void Application::handleMouseMove(const sf::Event& event)
{
	if (isAboveGUI(event.mouseMove.x, event.mouseMove.y))
	{
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_guiView);
		m_gui.handleMouseMove(position.x, position.y);
		m_showSelectedTile = false;
	}
	else
	{
		m_gui.resetButtons();
		if(isAboveMapArea(event.mouseMove.x, event.mouseMove.y))
		{
			if(m_dragging)
			{
				sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_view);
				sf::FloatRect rect;
				rect.left = std::min(m_dragStart.x, position.x);
				rect.top = std::min(m_dragStart.y, position.y);
				rect.width = std::abs(m_dragStart.x - position.x);
				rect.height = std::abs(m_dragStart.y - position.y);

				//horizonzal
				if (rect.left < 0)
				{
					rect.width = rect.left + rect.width;
					rect.left = 0;
				}
				else if ((rect.left + rect.width) >(m_mapSize.x * m_tileSize.x))
				{
					rect.width = (m_mapSize.x * m_tileSize.x) - rect.left - 1;
				}
				//vertical
				if (rect.top < 0)
				{
					rect.height = rect.top + rect.height;
					rect.top = 0;
				}
				else if ((rect.top + rect.height) >(m_mapSize.y * m_tileSize.y))
				{
					rect.height = (m_mapSize.y * m_tileSize.y) - rect.top - 1;
				}

				int realX, realY, realX2, realY2;

				realX = rect.left / m_tileSize.x;
				realY = rect.top / m_tileSize.y;
				realX2 = (rect.left + rect.width) / m_tileSize.x;
				realY2 = (rect.top + rect.height) / m_tileSize.y;

				m_selectedTilesShape.setPosition(realX * m_tileSize.x, realY * m_tileSize.x);
				m_selectedTilesShape.setSize(sf::Vector2f((1 + realX2 - realX) * m_tileSize.x, (1 + realY2 - realY) * m_tileSize.y));
				m_showSelectedTile = true;
			}
			else //not dragging
			{
				sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_view);

				if (aboveValidMapArea(position.x, position.y))
				{
					int realX, realY;
					realX = position.x / m_tileSize.x;
					realY = position.y / m_tileSize.y;

					if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
						return;

					m_showSelectedTile = true;
					m_selectedTilesShape.setPosition(realX * m_tileSize.x, realY * m_tileSize.y);
					m_selectedTilesShape.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));
				}
				else
					m_showSelectedTile = false;
			}
		}
	}
}

void Application::handleMouseButtonPress(const sf::Event& event)
{
	m_dragging = false;

	//Mouse over Map Area
	if (isAboveMapArea(event.mouseButton.x, event.mouseButton.y))
	{
		//calculate tile
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_view);

		//click inside map to start dragging
		if (!aboveValidMapArea(position.x, position.y))
			return;

		m_dragStart.x = position.x;
		m_dragStart.y= position.y;

		m_dragging = true;

	}
	//Mouse over GUI
	else if(isAboveGUI(event.mouseButton.x, event.mouseButton.y))
	{
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_guiView);
		m_gui.handleMouseClick(position.x, position.y, event.mouseButton.button);
	}
}

void Application::handleMouseButtonRelease(const sf::Event& event)
{
	if (isAboveMapArea(event.mouseButton.x, event.mouseButton.y) && m_dragging)
	{
		//calculate tile
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_view);

		if(aboveValidMapArea(position.x, position.y))
		{
			m_dragEnd.x = position.x;
			m_dragEnd.y = position.y;

			if(m_dragStart == m_dragEnd)
			{
				if (m_mode == GUI::Mode::TILE && m_currentTilePlacingValue >= 0)
					m_map.changeTileFromMousePosition(position.x, position.y, m_currentTilePlacingValue);
				else if (m_mode == GUI::Mode::COLLSION)
					m_map.changeCollisionTileFromMousePosition(position.x, position.y, m_collisionLayerPlaceCollisions);
			}
			else
			{
				sf::FloatRect rect;
				rect.left = std::min(m_dragStart.x, m_dragEnd.x);
				rect.top = std::min(m_dragStart.y, m_dragEnd.y);
				rect.width = std::abs(m_dragStart.x - m_dragEnd.x);
				rect.height = std::abs(m_dragStart.y - m_dragEnd.y);

				if (m_mode == GUI::Mode::TILE && m_currentTilePlacingValue >= 0)
					m_map.changeTilesFromRectangle(rect, m_currentTilePlacingValue);
				else if (m_mode == GUI::Mode::COLLSION)
					m_map.changeCollisionTilesFromRectangle(rect, m_collisionLayerPlaceCollisions);
			}

			int realX, realY;
			realX = position.x / m_tileSize.x;
			realY = position.y / m_tileSize.y;

			if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
				return;

			m_selectedTilesShape.setPosition(realX * m_tileSize.x, realY * m_tileSize.y);
			m_selectedTilesShape.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));
		}
		else
		{
			m_showSelectedTile = false;

			m_dragEnd.x = position.x;
			m_dragEnd.y = position.y;

			if (m_dragStart != m_dragEnd)
			{
				sf::FloatRect rect;
				rect.left = std::min(m_dragStart.x, m_dragEnd.x);
				rect.top = std::min(m_dragStart.y, m_dragEnd.y);
				rect.width = std::abs(m_dragStart.x - m_dragEnd.x);
				rect.height = std::abs(m_dragStart.y - m_dragEnd.y);

				//horizonzal
				if(rect.left < 0)
				{
					rect.width = rect.left + rect.width;
					rect.left = 0;
				}
				else if((rect.left + rect.width) > (m_mapSize.x * m_tileSize.x))
				{
					rect.width = (m_mapSize.x * m_tileSize.x) - rect.left - 1;
				}
				//vertical
				if(rect.top < 0)
				{
					rect.height = rect.top + rect.height;
					rect.top = 0;
				}
				else if((rect.top + rect.height) > (m_mapSize.y * m_tileSize.y))
				{
					rect.height = (m_mapSize.y * m_tileSize.y) - rect.top - 1;
				}
				if (m_mode == GUI::Mode::TILE && m_currentTilePlacingValue >= 0)
					m_map.changeTilesFromRectangle(rect, m_currentTilePlacingValue);
				else if (m_mode == GUI::Mode::COLLSION)
					m_map.changeCollisionTilesFromRectangle(rect, m_collisionLayerPlaceCollisions);
			}
		}

	}

	m_dragging = false;
}
