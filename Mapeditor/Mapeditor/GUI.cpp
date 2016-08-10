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

	m_borderShapes[5].setSize(sf::Vector2f(8, 620));
	m_borderShapes[5].setPosition(240.f, 0.f);
	m_borderShapes[5].setFillColor(sf::Color::Black);




	//wegen ursprünglichem sf::text als button*
	m_buttons.push_back(new Button(8.f, 16.f, Button::ButtonAction::NEW, this));
	m_buttons.push_back(new Button(124.f, 16.f, Button::ButtonAction::LOAD, this));
	m_buttons.push_back(new Button(8.f, 79.f, Button::ButtonAction::CONFIG, this));
	m_buttons.push_back(new Button(124.f, 79.f, Button::ButtonAction::SAVE, this));
	m_buttons.push_back(new Button(8.f, 158.f, Button::ButtonAction::TILES, this));
	m_buttons.push_back(new Button(124.f, 158.f, Button::ButtonAction::COLLISION, this));


	//m_tileLayerButtons.push_back(new Button(57.f, 298.f, Button::ButtonAction::ARROW_LEFT, this));
	m_tileLayerButtons.push_back(new Button(116.f, 326.f, Button::ButtonAction::ARROW_LEFT, this));
	m_tileLayerButtons.push_back(new Button(178.f, 326.f, Button::ButtonAction::ARROW_RIGHT, this));

	m_collisionLayerButtons.push_back(new Button(8.f, 237.f, Button::ButtonAction::PLACE, this));
	m_collisionLayerButtons.push_back(new Button(124.f, 237.f, Button::ButtonAction::DELETE, this));

	m_tilesBackground.setPosition(8.f, 380.f);
	m_tilesBackground.setSize(sf::Vector2f(224.f, 224.f));
	m_tilesBackground.setFillColor(sf::Color(60, 60, 60));
	m_buttons[4]->deactivate();

	if (!m_noTileTexture.loadFromFile("resources/button_no_tile.png"))
		std::cerr << "GUI ctor error loading noTile texture!" << std::endl;

	//TILE BUTTONS
	int id = 0;

	m_tileButtons.push_back(new TileButton( 14.f, 386.f, &m_noTileTexture, id++, this)); //4 abstand zwischen tiles
	m_tileButtons.push_back(new TileButton( 68.f, 386.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 386.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 386.f, &m_noTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 14.f, 440.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 68.f, 440.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 440.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 440.f, &m_noTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 14.f, 494.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 68.f, 494.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 494.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 494.f, &m_noTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 14.f, 548.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 68.f, 548.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(122.f, 548.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(176.f, 548.f, &m_noTileTexture, id++, this));

	m_currentPlacingTile.setTexture(&m_noTileTexture);
	m_currentPlacingTile.setSize(sf::Vector2f(50.f, 50.f));
	m_currentPlacingTile.setPosition(41.f, 319.f);

	if (!m_frameTexture.loadFromFile("resources/label_current_tile.png"))
		std::cerr << "GUI ctor error loading frame texture!" << std::endl;
	m_currentPlacingTileFrame.setTexture(m_frameTexture);
	m_currentPlacingTileFrame.setPosition(38.f, 316.f);

	m_tileLayerButtons[0]->deactivate();
	m_buttons[1]->deactivate();
	m_buttons[2]->deactivate();
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
	for (auto b : m_tileLayerButtons)
		b->handleMouseMove(pos.x, pos.y);
	for (auto b : m_collisionLayerButtons)
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
		for (auto b : m_tileLayerButtons)
			if (b->isActive())
				b->handleMouseClick(pos.x, pos.y, button);
		for (auto b : m_tileButtons)
			b->handleMouseClick(pos.x, pos.y, button);
	}
	else if (m_mode == COLLSION)
	{
		for (auto b : m_collisionLayerButtons)
			if (b->isActive())
				b->handleMouseClick(pos.x, pos.y, button);
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
	for (auto button : m_tileLayerButtons)
		button->reset();
	for (auto button : m_collisionLayerButtons)
		button->reset();
	for (auto button : m_tileButtons)
		button->reset();
}

void GUI::update(sf::Time elapsedTime)
{
	for (auto button : m_buttons)
		button->update(elapsedTime);
	for (auto button : m_tileLayerButtons)
		button->update(elapsedTime);
	for (auto button : m_collisionLayerButtons)
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
		for (auto button : m_tileLayerButtons)
			target.draw(*button, states);

		for (auto button : m_tileButtons)
			target.draw(*button, states);

		target.draw(m_currentPlacingTile, states);
		target.draw(m_currentPlacingTileFrame, states);


		for (auto button : m_collisionLayerButtons)
			target.draw(*button, states);
	}
	else if(m_mode == COLLSION)
	{
		target.draw(m_tilesBackground, states);
		for (auto button : m_tileLayerButtons)
			target.draw(*button, states);

		for (auto button : m_tileButtons)
			target.draw(*button, states);

		target.draw(m_currentPlacingTile, states);
		target.draw(m_currentPlacingTileFrame, states);


		for (auto button : m_collisionLayerButtons)
			target.draw(*button, states);
	}
}

void GUI::updateTileButtons()
{
	int amountOfTiles = (m_tileset.getSize().x / m_tileSize.x) * (m_tileset.getSize().y / m_tileSize.y);

	for(int i = 0; i < 16; ++i)
	{
		if ((i + 16 * m_currentTilePage) >= amountOfTiles)
			m_tileButtons[i]->setTextureToNoTile();
		else
		{
			int x = ((i + 16 * m_currentTilePage) % (m_tileset.getSize().x / m_tileSize.x)) * m_tileSize.x;
			int y = ((i + 16 * m_currentTilePage) / (m_tileset.getSize().x / m_tileSize.x)) * m_tileSize.x;

			m_tileButtons[i]->setTextureRect(sf::IntRect(x, y, m_tileSize.x, m_tileSize.y));
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
			m_buttons[4]->deactivate();//TILES
			m_buttons[5]->activate();//COLLISION
		}
		else if (mode == COLLSION)
		{
			m_buttons[4]->activate();//TILES
			m_buttons[5]->deactivate();//COLLISION
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

	std::cout << "amount of tiles = " << m_amountOfDifferentTiles << std::endl;

	m_amountOfTilePages = m_amountOfDifferentTiles / 16;

	//Falls es keine ganzzahlige Division war
	float f1 = (static_cast<float>(m_amountOfDifferentTiles) / 16.f) - (m_amountOfDifferentTiles / 16);
	if (f1 != 0)
		m_amountOfTilePages++;

	std::cout << "amount of tilepages = " << m_amountOfTilePages << std::endl;

	for (int i = 0; i < 16; ++i)
		m_tileButtons[i]->setTexture(&m_tileset);

	updateTileButtons();
}

void GUI::nextPage()
{
	if ((m_currentTilePage + 1) == m_amountOfTilePages)
		return;
	++m_currentTilePage;
	updateTileButtons();

	m_tileLayerButtons[0]->activate();

	if ((m_currentTilePage + 1) == m_amountOfTilePages)
		m_tileLayerButtons[1]->deactivate();
}

void GUI::previousPage()
{
	if (m_currentTilePage == 0)
		return;
	--m_currentTilePage;
	updateTileButtons();

	m_tileLayerButtons[1]->activate();

	for (auto button : m_tileButtons)
		button->setTexture(&m_tileset);

	if (m_currentTilePage == 0)
		m_tileLayerButtons[0]->deactivate();
}
