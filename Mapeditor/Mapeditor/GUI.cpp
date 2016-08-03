#include "GUI.hpp"
#include <iostream>

GUI::GUI(unsigned width, unsigned height)
{
	m_borderShapes[0].setSize(sf::Vector2f(10.f, height));
	m_borderShapes[0].setPosition(250.f, 0.f);
	m_borderShapes[0].setFillColor(sf::Color::Black);

	m_borderShapes[1].setSize(sf::Vector2f(250, 10.f));
	m_borderShapes[1].setPosition(0.f, 140.f);
	m_borderShapes[1].setFillColor(sf::Color::Black);

	//wegen ursprünglichem sf::text als button*
	m_buttons.push_back(new Button(10.f, 10.f, "ressources/button_new.png", Button::ButtonAction::NEW));
	m_buttons.push_back(new Button(130.f, 10.f, "ressources/button_load.png", Button::ButtonAction::LOAD));
	m_buttons.push_back(new Button(10.f, 75.f, "ressources/button_config.png", Button::ButtonAction::CONFIG));
	m_buttons.push_back(new Button(130.f, 75.f, "ressources/button_save.png", Button::ButtonAction::SAVE));
}

GUI::~GUI()
{
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		delete *it;
}

void GUI::handleMouseClick(float x, float y, int button)
{
	sf::Vector2f pos(x, y);
	//std::cout << "handleMouseClick before trans: x=" << x << " y=" << y;
	pos = getTransform().getInverse().transformPoint(pos);
	//std::cout << "handleMouseClick after trans: x=" << pos.x << " y=" << pos.y << std::endl;
	for (auto b : m_buttons)
		b->handleMouseClick(pos.x, pos.y, button);
}

void GUI::resize(unsigned width, unsigned height)
{
	m_borderShapes[0].setSize(sf::Vector2f(10.f, height));
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_borderShapes[0], states);
	target.draw(m_borderShapes[1], states);

	for (auto button : m_buttons)
		target.draw(*button, states);
}