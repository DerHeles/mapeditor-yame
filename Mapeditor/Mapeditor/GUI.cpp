#include "GUI.hpp"
#include <iostream>
#include "Application.hpp"

GUI::GUI(unsigned width, unsigned height, Application *application)
	:
	m_mode(Mode::TILE),
	m_application(application)
{
	m_borderShapes[0].setSize(sf::Vector2f(10.f, height));
	m_borderShapes[0].setPosition(250.f, 0.f);
	m_borderShapes[0].setFillColor(sf::Color::Black);

	m_borderShapes[1].setSize(sf::Vector2f(250, 10.f));
	m_borderShapes[1].setPosition(0.f, 140.f);
	m_borderShapes[1].setFillColor(sf::Color::Black);

	m_borderShapes[2].setSize(sf::Vector2f(250, 10.f));
	m_borderShapes[2].setPosition(0.f, 265.f);
	m_borderShapes[2].setFillColor(sf::Color::Black);

	//wegen ursprünglichem sf::text als button*
	m_buttons.push_back(new Button(10.f, 10.f, Button::ButtonAction::NEW, this));
	m_buttons.push_back(new Button(130.f, 10.f, Button::ButtonAction::LOAD, this));
	m_buttons.push_back(new Button(10.f, 75.f, Button::ButtonAction::CONFIG, this));
	m_buttons.push_back(new Button(130.f, 75.f, Button::ButtonAction::SAVE, this));
	m_buttons.push_back(new Button(10.f, 200.f, Button::ButtonAction::TILES, this));
	m_buttons.push_back(new Button(130.f, 200.f, Button::ButtonAction::COLLISION, this));


	m_tileLayerButtons.push_back(new Button(57.f, 298.f, Button::ButtonAction::ARROW_LEFT, this));
	m_tileLayerButtons.push_back(new Button(162.f, 298.f, Button::ButtonAction::ARROW_RIGHT, this));

	m_collisionLayerButtons.push_back(new Button(10.f, 285.f, Button::ButtonAction::PLACE, this));
	m_collisionLayerButtons.push_back(new Button(130.f, 285.f, Button::ButtonAction::DELETE, this));

	if (!m_layerTexture.loadFromFile("resources/label_layers.png"))
		std::cerr << "GUI ctor error loading layer texture!" << std::endl;
	m_layerSprite.setTexture(m_layerTexture);
	m_layerSprite.setPosition(80, 160);

	m_tilesBackground.setPosition(10.f, 350.f);
	m_tilesBackground.setSize(sf::Vector2f(230.f, 230.f));
	m_tilesBackground.setFillColor(sf::Color(60, 60, 60));
	m_buttons[4]->setInactive();

	if (!m_noTileTexture.loadFromFile("resources/button_no_tile.png"))
		std::cerr << "GUI ctor error loading noTile texture!" << std::endl;

	//TILE BUTTONS
	int id = 0;

	m_tileButtons.push_back(new TileButton( 16.f, 356.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 72.f, 356.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(128.f, 356.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(184.f, 356.f, &m_noTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 16.f, 412.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 72.f, 412.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(128.f, 412.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(184.f, 412.f, &m_noTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 16.f, 468.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 72.f, 468.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(128.f, 468.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(184.f, 468.f, &m_noTileTexture, id++, this));

	m_tileButtons.push_back(new TileButton( 16.f, 524.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton( 72.f, 524.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(128.f, 524.f, &m_noTileTexture, id++, this));
	m_tileButtons.push_back(new TileButton(184.f, 524.f, &m_noTileTexture, id++, this));
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
	m_borderShapes[0].setSize(sf::Vector2f(10.f, height));
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
	target.draw(m_borderShapes[0], states);
	target.draw(m_borderShapes[1], states);
	target.draw(m_borderShapes[2], states);

	for (auto button : m_buttons)
		target.draw(*button, states);

	target.draw(m_layerSprite, states);

	if(m_mode == TILE)
	{
		target.draw(m_tilesBackground, states);
		for (auto button : m_tileLayerButtons)
			target.draw(*button, states);

		for (auto button : m_tileButtons)
			target.draw(*button, states);
	}
	else if(m_mode == COLLSION)
	{
		for (auto button : m_collisionLayerButtons)
			target.draw(*button, states);
	}
}

void GUI::changeToMode(Mode mode)
{
	if(m_mode != mode)
	{
		m_mode = mode;
		if (mode == TILE)
		{
			m_buttons[4]->setInactive();//TILES
			m_buttons[5]->setActive();//COLLISION
		}
		else if (mode == COLLSION)
		{
			m_buttons[4]->setActive();//TILES
			m_buttons[5]->setInactive();//COLLISION
		}
	}
}

void GUI::changeCurrentPlacingTile(int tilePlaceValue)
{
	m_application->changeCurrentPlacingTile(tilePlaceValue);
}
