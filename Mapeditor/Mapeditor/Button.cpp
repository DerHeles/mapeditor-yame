#include "Button.hpp"
#include <iostream>
#include "GUI.hpp"
#include "Application.hpp"

Button::Button(float x, float y, ButtonAction action, GUI *gui, Application *app)
	:
	m_action(action),
	m_active(true),
	m_clicked(false),
	m_mouseOver(false),
	m_gui(gui),
	m_app(app)
{
	setPosition(x, y);
	std::string activeTexture = "resources/";
	std::string inactiveTexture = "resources/";
	switch(action)
	{
	case ButtonAction::NEW:
		activeTexture.append("new.png");
		inactiveTexture.append("new_gray.png");
		break;
	case ButtonAction::SAVE:
		activeTexture.append("save.png");
		inactiveTexture.append("save_gray.png");
		break;
	case ButtonAction::LOAD:
		activeTexture.append("load.png");
		inactiveTexture.append("load_gray.png");
		break;
	case ButtonAction::CONFIG:
		activeTexture.append("config.png");
		inactiveTexture.append("config_gray.png");
		break;
	case ButtonAction::TILE_VIEW:
		activeTexture.append("tile_view.png");
		inactiveTexture.append("tile_view_gray.png");
		break;
	case ButtonAction::COLLISION_VIEW:
		activeTexture.append("collision_view.png");
		inactiveTexture.append("collision_view_gray.png");
		break;
	case ButtonAction::ARROW_LEFT:
		activeTexture.append("arrow_left.png");
		inactiveTexture.append("arrow_left_gray.png");
		break;
	case ButtonAction::ARROW_RIGHT:
		activeTexture.append("arrow_right.png");
		inactiveTexture.append("arrow_right_gray.png");
		break;
	case ButtonAction::EMPTY_TILE:
		activeTexture.append("empty_tile.png");
		inactiveTexture.append("empty_tile_gray.png");
		break;
	case ButtonAction::DELETE:
		activeTexture.append("delete.png");
		inactiveTexture.append("delete_gray.png");
		break;
	case ButtonAction::PLACE:
		activeTexture.append("place.png");
		inactiveTexture.append("place_gray.png");
		break;
	case ButtonAction::NO_ACTION:
		return;
		break;
	default: break;
	}
	if (!m_activeTexture.loadFromFile(activeTexture))
		std::cerr << "Button could not load active texture!" << std::endl;
	if (!m_inactiveTexture.loadFromFile(inactiveTexture))
		std::cerr << "Button could not load inactive texture!" << std::endl;

	m_sprite.setTexture(m_activeTexture);
}

void Button::handleMouseMove(float x, float y)
{
	sf::Vector2f v = getTransform().getInverse().transformPoint(x, y);

	if (m_sprite.getGlobalBounds().contains(v) && m_active)
	{
		m_sprite.setColor(sf::Color(255, 219, 137));
		m_mouseOver = true;
	}
	else
	{
		m_sprite.setColor(sf::Color::White);
		m_mouseOver = false;
	}
}

void Button::handleMouseClick(float x, float y, int button)
{
	sf::Vector2f v = getTransform().getInverse().transformPoint(x, y);

	if (m_sprite.getGlobalBounds().contains(v))
	{
		if(button == 0)
		{
			m_clicked = true;
			m_clickedTime = sf::seconds(0.18f);
			m_sprite.setColor(sf::Color(255, 245, 201));
			switch (m_action)
			{
			case ButtonAction::NEW:
				std::cout << "NEW" << std::endl;
				m_app->createMap();
				break;
			case ButtonAction::LOAD:
				std::cout << "LOAD" << std::endl;
				m_app->loadMap();
				break;
			case ButtonAction::SAVE:
				std::cout << "SAVE" << std::endl;
				m_app->saveMap();
				break;
			case ButtonAction::CONFIG:
				std::cout << "CONFIG" << std::endl;
				m_app->configMap();
				break;
			case ButtonAction::TILE_VIEW:
				std::cout << "TILE_VIEW" << std::endl;
				m_gui->changeToMode(GUI::Mode::TILE);
				break;
			case ButtonAction::COLLISION_VIEW:
				std::cout << "COLLISION_VIEW" << std::endl;
				m_gui->changeToMode(GUI::Mode::COLLSION);
				break;
			case ButtonAction::ARROW_LEFT:
				std::cout << "ARROW_LEFT" << std::endl;
				m_gui->previousPage();
				break;
			case ButtonAction::ARROW_RIGHT:
				std::cout << "ARROW_RIGHT" << std::endl;
				m_gui->nextPage();
				break;
			case ButtonAction::EMPTY_TILE:
				std::cout << "EMPTY_TILE" << std::endl;
				break;
			case ButtonAction::DELETE:
				std::cout << "DELETE" << std::endl;
				m_gui->changeCollisionPlacing(false);
				break;
			case ButtonAction::PLACE:
				std::cout << "PLACE" << std::endl;
				m_gui->changeCollisionPlacing(true);
				break;
			case ButtonAction::NO_ACTION:
				std::cout << "NO_ACTION" << std::endl;
				break;
			default:
				std::cout << "no suitable action" << std::endl;
				break;
			}
		}
	}
}

void Button::reset()
{
	m_sprite.setColor(sf::Color::White);
}

void Button::update(sf::Time elapsedTime)
{
	if(m_clicked)
	{
		m_clickedTime -= elapsedTime;
		if (m_clickedTime <= sf::Time::Zero)
		{
			m_clicked = false;
			if(m_mouseOver && m_active)
				m_sprite.setColor(sf::Color(255, 219, 137));
			else
				m_sprite.setColor(sf::Color::White);
		}
	}
}

void Button::activate()
{
	m_active = true;
	m_sprite.setTexture(m_activeTexture);
}

void Button::deactivate()
{
	m_active = false;
	m_sprite.setTexture(m_inactiveTexture);
}

bool Button::isActive()
{
	return m_active;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}
