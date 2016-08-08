#include "TileButton.hpp"
#include "GUI.hpp"

TileButton::TileButton(float x, float y, sf::Texture *startTexture, int tileID, GUI *gui)
	:
	m_gui(gui),
	m_currentTileID(tileID)
{
	setPosition(x, y);
	m_shape.setSize(sf::Vector2f(50.f, 50.f));
	m_shape.setTexture(startTexture);

	m_noTileTexture = startTexture;
}

void TileButton::handleMouseMove(float x, float y)
{
}

void TileButton::handleMouseClick(float x, float y, int button)
{
	sf::Vector2f v = getTransform().getInverse().transformPoint(x, y);

	if (m_shape.getGlobalBounds().contains(v))
	{
		if (button == 0)
		{
			m_gui->changeCurrentPlacingTile(m_currentTileID, m_shape.getTextureRect());
		}
	}
}

void TileButton::reset()
{
}

void TileButton::update(sf::Time elapsedTime)
{
}

void TileButton::setTexture(sf::Texture* texture)
{
	m_shape.setTexture(texture);
}

void TileButton::setTextureRect(sf::IntRect rect)
{
	m_shape.setTextureRect(rect);
}

void TileButton::setTextureToNoTile()
{
	m_shape.setTexture(m_noTileTexture, true);
	m_active = false;
}

void TileButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_shape, states);
}
