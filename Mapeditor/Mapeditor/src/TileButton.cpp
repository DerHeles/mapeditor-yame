#include "TileButton.hpp"
#include "GUI.hpp"

TileButton::TileButton(float x, float y, sf::Texture *noTileTexture, sf::Texture *deactivatedTexture, int tileID, GUI *gui)
	:
	m_gui(gui),
	m_currentTileID(tileID),
	m_showingTile(false)
{
	setPosition(x, y);
	m_shape.setSize(sf::Vector2f(50.f, 50.f));
	m_shape.setTexture(noTileTexture);

	m_noTileTexture = noTileTexture;
	m_deactivatedTileTexture = deactivatedTexture;
}

void TileButton::handleMouseClick(float x, float y, int button) const
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

void TileButton::activate()
{
	m_active = true;
	if(m_showingTile)
	{
		m_shape.setTexture(m_tileset);
		m_shape.setTextureRect(m_texRect);
	}
	else
	{
		m_shape.setTexture(m_noTileTexture, true);
	}
}

void TileButton::deactivate()
{
	m_active = false;
	m_shape.setTexture(m_deactivatedTileTexture, true);
}

bool TileButton::isActive() const
{
	return m_active;
}

void TileButton::setTileset(sf::Texture* texture)
{
	m_tileset = texture;
}

void TileButton::showTile()
{
	m_shape.setTexture(m_tileset);
	m_shape.setTextureRect(m_texRect);
	m_showingTile = true;
}

void TileButton::showNoTile()
{
	m_shape.setTexture(m_noTileTexture, true);
	m_showingTile = false;
}

void TileButton::setTileTextureRect(const sf::IntRect& rect)
{
	m_texRect = rect;
}

void TileButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_shape, states);
}
