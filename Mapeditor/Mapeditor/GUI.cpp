#include "GUI.hpp"
#include <iostream>
#include "Application.hpp"
#include "Helper.hpp"

GUI::GUI(unsigned int width, unsigned int height, Application *application)
	:
	m_mode(Mode::TILE),
	m_application(application)
{
	m_borderShapes[0].setSize(sf::Vector2f(240.f, 8.f));
	m_borderShapes[0].setPosition(0.f, 0.f);
	m_borderShapes[0].setFillColor(sf::Color::Black);

	m_borderShapes[1].setSize(sf::Vector2f(240, 8.f));
	m_borderShapes[1].setPosition(0.f, 142.f);
	m_borderShapes[1].setFillColor(sf::Color::Black);

	m_borderShapes[2].setSize(sf::Vector2f(240, 8.f));
	m_borderShapes[2].setPosition(0.f, 221.f);
	m_borderShapes[2].setFillColor(sf::Color::Black);

	m_borderShapes[3].setSize(sf::Vector2f(240, 8.f));
	m_borderShapes[3].setPosition(0.f, 300.f);
	m_borderShapes[3].setFillColor(sf::Color::Black);

	m_borderShapes[4].setSize(sf::Vector2f(240, 8.f));
	m_borderShapes[4].setPosition(0.f, 612.f);
	m_borderShapes[4].setFillColor(sf::Color::Black);

	m_borderShapes[5].setSize(sf::Vector2f(8, hlp::gui_height));
	m_borderShapes[5].setPosition(240.f, 0.f);
	m_borderShapes[5].setFillColor(sf::Color::Black);




	//wegen ursprünglichem sf::text als button*
	m_buttons.push_back(new Button(8.f, 16.f, Button::ButtonAction::NEW, this));
	m_buttons.push_back(new Button(124.f, 16.f, Button::ButtonAction::LOAD, this));
	m_buttons.push_back(new Button(8.f, 79.f, Button::ButtonAction::CONFIG, this));
	m_buttons.push_back(new Button(124.f, 79.f, Button::ButtonAction::SAVE, this));
	m_buttons.push_back(new Button(8.f, 158.f, Button::ButtonAction::TILE_VIEW, this));
	m_buttons.push_back(new Button(124.f, 158.f, Button::ButtonAction::COLLISION_VIEW, this));


	m_buttons.push_back(new Button(8.f, 237.f, Button::ButtonAction::PLACE, this));
	m_buttons.push_back(new Button(124.f, 237.f, Button::ButtonAction::DELETE, this));

	m_buttons.push_back(new Button(116.f, 326.f, Button::ButtonAction::ARROW_LEFT, this));
	m_buttons.push_back(new Button(178.f, 326.f, Button::ButtonAction::ARROW_RIGHT, this));


	m_tilesBackground.setPosition(8.f, 380.f);
	m_tilesBackground.setSize(sf::Vector2f(224.f, 224.f));
	m_tilesBackground.setFillColor(sf::Color(60, 60, 60));

	if (!m_noTileTexture.loadFromFile("resources/button_no_tile.png"))
		std::cerr << "GUI ctor error loading noTile texture!" << std::endl;

	if (!m_deactivatedTileTexture.loadFromFile("resources/tile_deactived.png"))
		std::cerr << "GUI ctor error loading deactivatedTile texture!" << std::endl;

	//TILE BUTTONS
	int id = 0;

	m_tileButtons.push_back(new TileButton( 14.f, 386.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this)); //4 abstand zwischen tiles
	m_tileButtons.push_back(new TileButton( 68.f, 386.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 386.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 386.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 14.f, 440.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 68.f, 440.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 440.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 440.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 14.f, 494.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 68.f, 494.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 494.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 494.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 14.f, 548.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 68.f, 548.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 548.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 548.f, &m_noTileTexture, &m_deactivatedTileTexture, id++, this));

	m_currentPlacingTile.setTexture(&m_noTileTexture);
	m_currentPlacingTile.setSize(sf::Vector2f(50.f, 50.f));
	m_currentPlacingTile.setPosition(41.f, 319.f);

	if (!m_frameTexture.loadFromFile("resources/label_current_tile.png"))
		std::cerr << "GUI ctor error loading frame texture!" << std::endl;
	m_currentPlacingTileFrame.setTexture(m_frameTexture);
	m_currentPlacingTileFrame.setPosition(38.f, 316.f);

	m_deactivedPlacingTile.setTexture(m_deactivatedTileTexture);
	m_deactivedPlacingTile.setPosition(41.f, 319.f);

	//deactivate buttons
	m_buttons[index(ButtonID::ARROW_LEFT)]->deactivate();
	m_buttons[index(ButtonID::SAVE)]->deactivate();
	m_buttons[index(ButtonID::CONFIG)]->deactivate();
	m_buttons[index(ButtonID::TILE_VIEW)]->deactivate();
	m_buttons[index(ButtonID::PLACE)]->deactivate();
	m_buttons[index(ButtonID::DELETE)]->deactivate();

}

GUI::~GUI()
{
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		delete *it;
}

void GUI::handleMouseMove(float x, float y)
{
	sf::Vector2f pos(x, y);
	pos = getTransform().getInverse().transformPoint(pos);
	for (auto b : m_buttons)
		b->handleMouseMove(pos.x, pos.y);
}

void GUI::handleMouseClick(float x, float y, int button)
{
	sf::Vector2f pos(x, y);
	//std::cout << "handleMouseClick before trans: x=" << x << " y=" << y;
	pos = getTransform().getInverse().transformPoint(pos);
	//std::cout << "handleMouseClick after trans: x=" << pos.x << " y=" << pos.y << std::endl;
	for (auto b : m_buttons)
		if(b->isActive())
			b->handleMouseClick(pos.x, pos.y, button);

	if (m_mode == TILE)
	{
		for (auto b : m_tileButtons)
			b->handleMouseClick(pos.x, pos.y, button);
	}
	else if (m_mode == COLLSION)
	{

	}
}

void GUI::resize(unsigned int width, unsigned int height)
{
	//m_borderShapes[0].setSize(sf::Vector2f(8.f, height));
}

void GUI::resetButtons()
{
	for (auto button : m_buttons)
		button->reset();
	for (auto button : m_tileButtons)
		button->reset();
}

void GUI::update(sf::Time elapsedTime)
{
	for (auto button : m_buttons)
		button->update(elapsedTime);
	for (auto button : m_tileButtons)
		button->update(elapsedTime);
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for(int i = 0; i < 6; ++i)
		target.draw(m_borderShapes[i], states);

	for (auto button : m_buttons)
		target.draw(*button, states);

	if(m_mode == TILE)
	{
		target.draw(m_tilesBackground, states);

		for (auto button : m_tileButtons)
			target.draw(*button, states);

		target.draw(m_currentPlacingTile, states);
		target.draw(m_currentPlacingTileFrame, states);

	}
	else if(m_mode == COLLSION)
	{
		target.draw(m_tilesBackground, states);

		for (auto button : m_tileButtons)
			target.draw(*button, states);

		target.draw(m_deactivedPlacingTile, states);
		target.draw(m_currentPlacingTileFrame, states);


	}
}

void GUI::updateTileButtons()
{
	int amountOfTiles = (m_tileset.getSize().x / m_tileSize.x) * (m_tileset.getSize().y / m_tileSize.y);

	for(int i = 0; i < 16; ++i)
	{
		if ((i + 16 * m_currentTilePage) >= amountOfTiles)
			m_tileButtons[i]->showNoTile();
		else
		{
			int x = ((i + 16 * m_currentTilePage) % (m_tileset.getSize().x / m_tileSize.x)) * m_tileSize.x;
			int y = ((i + 16 * m_currentTilePage) / (m_tileset.getSize().x / m_tileSize.x)) * m_tileSize.x;

			m_tileButtons[i]->setTileTextureRect(sf::IntRect(x, y, m_tileSize.x, m_tileSize.y));
			m_tileButtons[i]->showTile();
		}
	}
}

void GUI::changeToMode(Mode mode)
{
	if(m_mode != mode)
	{
		m_mode = mode;
		if (mode == TILE)
		{
			m_buttons[index(ButtonID::TILE_VIEW)]->deactivate();
			m_buttons[index(ButtonID::COLLISION_VIEW)]->activate();
			m_buttons[index(ButtonID::PLACE)]->deactivate();
			m_buttons[index(ButtonID::DELETE)]->deactivate();
			if(m_currentTilePage > 0)
				m_buttons[index(ButtonID::ARROW_LEFT)]->activate();
			if((m_currentTilePage + 1) < m_amountOfTilePages)
				m_buttons[index(ButtonID::ARROW_RIGHT)]->activate();

			m_application->changeToMode(Mode::TILE);

			for (auto b : m_tileButtons)
				b->activate();
		}
		else if (mode == COLLSION)
		{
			m_buttons[index(ButtonID::TILE_VIEW)]->activate();
			m_buttons[index(ButtonID::COLLISION_VIEW)]->deactivate();
			m_buttons[index(ButtonID::PLACE)]->activate();
			m_buttons[index(ButtonID::DELETE)]->activate();
			m_buttons[index(ButtonID::ARROW_LEFT)]->deactivate();
			m_buttons[index(ButtonID::ARROW_RIGHT)]->deactivate();

			m_application->changeToMode(Mode::COLLSION);

			for (auto b : m_tileButtons)
				b->deactivate();
		}
	}
}

void GUI::changeCurrentPlacingTile(int tilePlaceValue, sf::IntRect textureRect)
{
	if((tilePlaceValue + 16 * m_currentTilePage) < m_amountOfDifferentTiles)
	{
		m_application->changeCurrentPlacingTileValue(tilePlaceValue + 16 * m_currentTilePage);
		m_currentPlacingTile.setTexture(&m_tileset);
		m_currentPlacingTile.setTextureRect(textureRect);
	}
}

void GUI::loadTiles(const std::string& tileset, float tile_width, float tile_height)
{
	m_tileSize.x = tile_width;
	m_tileSize.y = tile_height;

	m_currentTilePage = 0;

	if (!m_tileset.loadFromFile(tileset))
		std::cerr << "loadTiles error loading tileset!" << std::endl;

	int tileset_width = m_tileset.getSize().x;
	int tileset_heigth = m_tileset.getSize().y;

	m_amountOfDifferentTiles = (tileset_width / tile_width) * (tileset_heigth / tile_height);

	m_amountOfTilePages = m_amountOfDifferentTiles / 16;

	//Falls es keine ganzzahlige Division war
	float f1 = (static_cast<float>(m_amountOfDifferentTiles) / 16.f) - (m_amountOfDifferentTiles / 16);
	if (f1 != 0)
		m_amountOfTilePages++;

	for (int i = 0; i < 16; ++i)
		m_tileButtons[i]->setTileset(&m_tileset);

	updateTileButtons();
}

void GUI::nextPage()
{
	if (m_currentTilePage + 1 == m_amountOfTilePages)
		return;
	++m_currentTilePage;
	updateTileButtons();

	m_buttons[index(ButtonID::ARROW_LEFT)]->activate();

	if (m_currentTilePage + 1 == m_amountOfTilePages)
		m_buttons[index(ButtonID::ARROW_RIGHT)]->deactivate();
}

void GUI::previousPage()
{
	if (m_currentTilePage == 0)
		return;
	--m_currentTilePage;
	updateTileButtons();

	m_buttons[index(ButtonID::ARROW_RIGHT)]->activate();

	for (auto button : m_tileButtons)
		button->showTile();

	if (m_currentTilePage == 0)
		m_buttons[index(ButtonID::ARROW_LEFT)]->deactivate();
}

int GUI::index(ButtonID id)
{
	return static_cast<std::underlying_type_t<ButtonID>>(id);
}
