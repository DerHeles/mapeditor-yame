#include "EditorTileMap.hpp"
#include <iostream>
#include <string>
#include <fstream>

EditorTileMap::EditorTileMap()
	:
	m_tiles(nullptr)
{
}

EditorTileMap::~EditorTileMap()
{
	delete[] m_tiles;
}

void EditorTileMap::changeCollisionTile(unsigned x, unsigned y, bool collision)
{
	m_collisionTiles[x + y * m_mapSize.x] = collision;
	updateCollisionVertices(x, y, collision);
}

void EditorTileMap::changeTile(unsigned int x, unsigned int y, int value)
{
	m_tiles[x + y * m_mapSize.x] = value;
	updateVertices(x, y);
}

bool EditorTileMap::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height, int tile_gap)
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	m_texturePath = tileset;

	m_mapSize.x = width;
	m_mapSize.y = height;
	m_tileSize = tileSize;

	m_tileGap = tile_gap;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get the current tile number
			int tileNumber = tiles[i + j * width];

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * (tileSize.x + tile_gap), tv * (tileSize.y + tile_gap));
			quad[1].texCoords = sf::Vector2f(tu * (tileSize.x + tile_gap) + tileSize.x, tv * (tileSize.y + tile_gap));
			quad[2].texCoords = sf::Vector2f(tu * (tileSize.x + tile_gap) + tileSize.x, tv * (tileSize.y + tile_gap) + tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * (tileSize.x + tile_gap), tv * (tileSize.y + tile_gap) + tileSize.y);
		}

	m_tiles = new int[width * height];
	for (int i = 0; i < width * height; ++i)
		m_tiles[i] = tiles[i];

	//collision layer
	m_collisionTiles = new bool[width * height];
	for (int i = 0; i < width * height; ++i)
		m_collisionTiles[i] = false;

	m_collisionVertices.setPrimitiveType(sf::Quads);
	m_collisionVertices.resize(width * height * 4);

	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_collisionVertices[(i + j * width) * 4];

			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 corners
			quad[0].color = sf::Color::Transparent;
			quad[1].color = sf::Color::Transparent;
			quad[2].color = sf::Color::Transparent;
			quad[3].color = sf::Color::Transparent;
		}

	return true;
}

void EditorTileMap::loadFromFile(std::ifstream* file)
{
	m_texturePath = readString(file);

	unsigned int tileSizeX, tileSizeY;
	file->read(reinterpret_cast<char*>(&tileSizeX), sizeof(tileSizeX));
	file->read(reinterpret_cast<char*>(&tileSizeY), sizeof(tileSizeY));

	unsigned int mapSizeX, mapSizeY;
	file->read(reinterpret_cast<char*>(&mapSizeX), sizeof(mapSizeX));
	file->read(reinterpret_cast<char*>(&mapSizeY), sizeof(mapSizeY));

	int *tiles = new int[mapSizeX * mapSizeY];
	file->read(reinterpret_cast<char*>(tiles), sizeof(int) * mapSizeX * mapSizeY);

	unsigned int tileGap;
	file->read(reinterpret_cast<char*>(&tileGap), sizeof(tileGap));

	load(m_texturePath, sf::Vector2u(tileSizeX, tileSizeY), tiles, mapSizeX, mapSizeY, tileGap);


	delete[] tiles;
}

void EditorTileMap::saveToFile(std::ofstream* file) const
{
	writeString(file, m_texturePath);

	file->write(reinterpret_cast<const char*>(&m_tileSize.x), sizeof(m_tileSize.x));
	file->write(reinterpret_cast<const char*>(&m_tileSize.y), sizeof(m_tileSize.y));

	file->write(reinterpret_cast<const char*>(&m_mapSize.x), sizeof(m_mapSize.x));
	file->write(reinterpret_cast<const char*>(&m_mapSize.y), sizeof(m_mapSize.y));

	file->write(reinterpret_cast<const char*>(m_tiles), sizeof(int) * m_mapSize.x * m_mapSize.y);

	file->write(reinterpret_cast<const char*>(&m_tileGap), sizeof(m_tileGap));
}

const sf::Vector2u& EditorTileMap::getTileSize() const
{
	return m_tileSize;
}

const sf::Vector2u& EditorTileMap::getMapSize() const
{
	return m_mapSize;
}

const std::string& EditorTileMap::getTexturePath() const
{
	return m_texturePath;
}

int EditorTileMap::getTileGap() const
{
	return m_tileGap;
}

void EditorTileMap::showCollisionLayer()
{
	m_showCollisionLayer = true;
}

void EditorTileMap::hideCollisionLayer()
{
	m_showCollisionLayer = false;
}

void EditorTileMap::writeString(std::ofstream* file, const std::string& str) const
{
	// get the length of the string data
	unsigned int len = str.size();

	// write the size:
	file->write(reinterpret_cast<const char*>(&len), sizeof(len));

	// write the actual string data:
	file->write(str.c_str(), len);
}

std::string EditorTileMap::readString(std::ifstream* file)
{
	// this probably isn't the optimal way to do it, but whatever
	std::string str;

	// get the length
	unsigned int len;
	file->read(reinterpret_cast<char*>(&len), sizeof(len));

	// we can't read to string directly, so instead, create a temporary buffer
	if (len > 0)
	{
		char* buf = new char[len];
		file->read(buf, len);
		str.append(buf, len);
		delete[] buf;
	}
	return str;
}

void EditorTileMap::changeTileFromMousePosition(float x, float y, int value)
{
	int realX, realY;
	realX = x / m_tileSize.x;
	realY = y / m_tileSize.y;
	if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
		return;
	changeTile(realX, realY, value);

}

void EditorTileMap::changeCollisionTileFromMousePosition(float x, float y, bool collision)
{
	int realX, realY;
	realX = x / m_tileSize.x;
	realY = y / m_tileSize.y;
	if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
		return;
	changeCollisionTile(realX, realY, collision);
}

void EditorTileMap::changeTilesFromRectangle(sf::FloatRect rect, int value)
{
	int realX, realY, realX2, realY2;

	realX = rect.left / m_tileSize.x;
	realY = rect.top / m_tileSize.y;
	realX2 = (rect.left + rect.width) / m_tileSize.x;
	realY2 = (rect.top + rect.height) / m_tileSize.y;

	if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
		return;

	if (realX2 < 0 || realX2 >= m_mapSize.x || realY2 < 0 || realY2 >= m_mapSize.y)
		return;

	for(int i = realX; i <= realX2; ++i)
		for(int j = realY; j <= realY2; ++j)
			changeTile(i, j, value);
}

void EditorTileMap::changeCollisionTilesFromRectangle(sf::FloatRect rect, bool collision)
{
	int realX, realY, realX2, realY2;

	realX = rect.left / m_tileSize.x;
	realY = rect.top / m_tileSize.y;
	realX2 = (rect.left + rect.width) / m_tileSize.x;
	realY2 = (rect.top + rect.height) / m_tileSize.y;

	if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
		return;

	if (realX2 < 0 || realX2 >= m_mapSize.x || realY2 < 0 || realY2 >= m_mapSize.y)
		return;

	for (int i = realX; i <= realX2; ++i)
		for (int j = realY; j <= realY2; ++j)
			changeCollisionTile(i, j, collision);
}

bool EditorTileMap::reload(const std::string& tileset, sf::Vector2u tileSize, unsigned int tile_gap)
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	m_texturePath = tileset;

	m_tileSize = tileSize;
	m_tileGap = tile_gap;

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < m_mapSize.x; ++i)
		for (unsigned int j = 0; j < m_mapSize.y; ++j)
		{
			// get the current tile number
			int tileNumber = m_tiles[i + j * m_mapSize.x];

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * m_mapSize.x) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap), tv * (m_tileSize.y + m_tileGap));
			quad[1].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap) + m_tileSize.x, tv * (m_tileSize.y + m_tileGap));
			quad[2].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap) + m_tileSize.x, tv * (m_tileSize.y + m_tileGap) + m_tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap), tv * (m_tileSize.y + m_tileGap) + m_tileSize.y);
		}
	return true;
}

void EditorTileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);

	if (m_showCollisionLayer)
		target.draw(m_collisionVertices, states);
}

void EditorTileMap::updateVertices(unsigned int x, unsigned int y)
{
	// get the current tile number
	int tileNumber = m_tiles[x + y * m_mapSize.x];

	// find its position in the tileset texture
	int tu = tileNumber % (m_tileset.getSize().x / m_tileSize.x);
	int tv = tileNumber / (m_tileset.getSize().x / m_tileSize.x);

	// get a pointer to the current tile's quad
	sf::Vertex* quad = &m_vertices[(x + y * m_mapSize.x) * 4];

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap), tv * (m_tileSize.y + m_tileGap));
	quad[1].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap) + m_tileSize.x, tv * (m_tileSize.y + m_tileGap));
	quad[2].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap) + m_tileSize.x, tv * (m_tileSize.y + m_tileGap) + m_tileSize.y);
	quad[3].texCoords = sf::Vector2f(tu * (m_tileSize.x + m_tileGap), tv * (m_tileSize.y + m_tileGap) + m_tileSize.y);
}

void EditorTileMap::updateCollisionVertices(unsigned int x, unsigned int y, bool collision)
{
	// get a pointer to the current tile's quad
	sf::Vertex* quad = &m_collisionVertices[(x + y * m_mapSize.x) * 4];
	if (collision)
	{
		quad[0].color = sf::Color(0, 0, 255, 140);
		quad[1].color = sf::Color(0, 0, 255, 140);
		quad[2].color = sf::Color(0, 0, 255, 140);
		quad[3].color = sf::Color(0, 0, 255, 140);
	}
	else
	{
		quad[0].color = sf::Color::Transparent;
		quad[1].color = sf::Color::Transparent;
		quad[2].color = sf::Color::Transparent;
		quad[3].color = sf::Color::Transparent;
	}
}