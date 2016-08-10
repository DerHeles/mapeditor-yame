#include "Application.hpp"
#include "Configuration.hpp"
#include <iostream>
#include "Helper.hpp"

Application::Application(unsigned width, unsigned height)
	:
	m_window(sf::VideoMode(width, height), "YAME - Yet Another Map Editor (c) Tobias Heiles", sf::Style::Default),
	m_gui(width, height, this),
	m_dragging(false),
	m_currentTilePlacingValue(-1),
	m_showSelectedTile(false)
{
	m_window.setFramerateLimit(60);


	//TileMap
	const int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 6, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 1, 1, 2, 2, 1, 1, 1, 1, 6, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 6, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 6, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 1, 1, 2, 2, 1, 1, 1, 1, 6, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1,
		0, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 3, 1, 2, 2, 1, 1,
		0, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 7,
	};
	m_map.load("tileset.png", sf::Vector2u(50, 50), level, 21, 16);
	m_view.setSize(m_window.getSize().x - 250.f, m_window.getSize().y);
	m_view.setViewport(sf::FloatRect(1.f - (static_cast<float>(width) - 250.f) / static_cast<float>(width), 0.f, (static_cast<float>(width) - 250.f) / static_cast<float>(width), 1.f));

	//GUI
	m_guiView = m_window.getView();
	m_gui.loadTiles("tileset.png", 50, 50);

	m_selectedTilesShape.setSize(sf::Vector2f(50, 50));
	m_selectedTilesShape.setFillColor(sf::Color(255, 100, 100, 180));

	m_tileSize.x = 50;
	m_tileSize.y = 50;

	m_mapSize.x = 21;
	m_mapSize.y = 16;
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
					if (event.size.height < 600)
						m_window.setSize(sf::Vector2u(800, 600));
					else
						m_window.setSize(sf::Vector2u(800, event.size.height));
				}
				if (event.size.height < 600)
					m_window.setSize(sf::Vector2u(event.size.width, 600));

				m_guiView.setSize(m_window.getSize().x, m_window.getSize().y);
				m_gui.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0,0), m_guiView));

				m_view.setViewport(sf::FloatRect(1.f - (static_cast<float>(m_window.getSize().x) - 250.f) / static_cast<float>(m_window.getSize().x), 0.f, (static_cast<float>(m_window.getSize().x) - 250.f) / static_cast<float>(m_window.getSize().x), 1.f));
				m_view.setSize(m_window.getSize().x - 250.f, m_window.getSize().y);

				m_gui.resize(event.size.width, event.size.height);

				break;
			default:
				;
		}
	}
}


void Application::handleMouseWheelScroll(const sf::Event& event)
{
	//std::cout << "MouseWheelScrolled delta=" << event.mouseWheelScroll.delta << std::endl;
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
	return (hlp::gui_width < x && x < m_window.getSize().x && 0 < y && y < m_window.getSize().y);
}

bool Application::isAboveGUI(int x, int y) const
{
	return (0 < x && x <= hlp::gui_width && 0 < y && y < m_window.getSize().y);
}

bool Application::aboveValidMapArea(float x, float y) const
{
	if (x < 0 || x >= (m_mapSize.x * m_tileSize.x) || y < 0 || y >= (m_mapSize.y * m_tileSize.y))
		return false;
	return true;
}

void Application::extractTilesFromTileset(const std::string& tileset, sf::Vector2u tileSize)
{
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
				//vertikal
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
	m_dragging = false; //nötig?

	//Mouse over Map Area
	if (isAboveMapArea(event.mouseButton.x, event.mouseButton.y))
	{
		//calculate tile
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_view);

		//innerhalb der map klicken um dragging zu starten
		if (!aboveValidMapArea(position.x, position.y))
			return;

		std::cout << "PRESS" << std::endl;
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

			if(m_currentTilePlacingValue >= 0)//nur falls auch TileWert ausgewählt wurde
			{
				if(m_dragStart == m_dragEnd)
					m_map.changeTileFromMousePosition(position.x, position.y, m_currentTilePlacingValue);
				else
				{
					sf::FloatRect rect;
					rect.left = std::min(m_dragStart.x, m_dragEnd.x);
					rect.top = std::min(m_dragStart.y, m_dragEnd.y);
					rect.width = std::abs(m_dragStart.x - m_dragEnd.x);
					rect.height = std::abs(m_dragStart.y - m_dragEnd.y);

					m_map.changeTilesFromRectangle(rect, m_currentTilePlacingValue);
				}
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

			if (m_currentTilePlacingValue >= 0)//nur falls auch TileWert ausgewählt wurde
			{
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
					//vertikal
					if(rect.top < 0)
					{
						rect.height = rect.top + rect.height;
						rect.top = 0;
					}
					else if((rect.top + rect.height) > (m_mapSize.y * m_tileSize.y))
					{
						rect.height = (m_mapSize.y * m_tileSize.y) - rect.top - 1;
					}
					m_map.changeTilesFromRectangle(rect, m_currentTilePlacingValue);
				}
			}
		}

	}
	//Mouse over GUI
	else if (isAboveGUI(event.mouseButton.x, event.mouseButton.y))
	{
		/*m_showSelectedTile = false;
		m_selectedTilesShape.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));*/
	}

	m_dragging = false;
}
