#include "TileMap.hpp"
#include <iostream>

TileMap::TileMap()
	:
	m_tiles(nullptr)
{
}

TileMap::~TileMap()
{
	delete[] m_tiles;
}

void TileMap::changeTile(unsigned x, unsigned y, int value)
{
	m_tiles[x + y * m_mapSize.x] = value;
	updateVertices(x, y);
}

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	m_texturePath = tileset;

	m_mapSize.x = width;
	m_mapSize.y = height;
	m_tileSize = tileSize;

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
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	m_tiles = new int[width * height];
	for (int i = 0; i < width * height; ++i)
		m_tiles[i] = tiles[i];
	return true;
}

void TileMap::loadFromFile(std::ifstream* file)
{
	m_texturePath = readString(file);
	std::cout << m_texturePath << std::endl;
	unsigned int tileSizeX, tileSizeY;
	file->read(reinterpret_cast<char*>(&tileSizeX), sizeof(tileSizeX));
	file->read(reinterpret_cast<char*>(&tileSizeY), sizeof(tileSizeY));
	unsigned int mapSizeX, mapSizeY;
	file->read(reinterpret_cast<char*>(&mapSizeX), sizeof(mapSizeX));
	file->read(reinterpret_cast<char*>(&mapSizeY), sizeof(mapSizeY));
	int *tiles = new int[mapSizeX * mapSizeY];
	file->read(reinterpret_cast<char*>(tiles), sizeof(int) * mapSizeX * mapSizeY);
	load(m_texturePath, sf::Vector2u(tileSizeX, tileSizeY), tiles, mapSizeX, mapSizeY);
	delete[] tiles;
}

void TileMap::saveToFile(std::ofstream* file) const
{
	writeString(file, m_texturePath);
	file->write(reinterpret_cast<const char*>(&m_tileSize.x), sizeof(m_tileSize.x));
	file->write(reinterpret_cast<const char*>(&m_tileSize.y), sizeof(m_tileSize.y));

	file->write(reinterpret_cast<const char*>(&m_mapSize.x), sizeof(m_mapSize.x));
	file->write(reinterpret_cast<const char*>(&m_mapSize.y), sizeof(m_mapSize.y));

	file->write(reinterpret_cast<const char*>(m_tiles), sizeof(int) * m_mapSize.x * m_mapSize.y);
}

void TileMap::writeString(std::ofstream* file, const std::string& str) const
{
	// get the length of the string data
	unsigned len = str.size();

	// write the size:
	file->write(reinterpret_cast<const char*>(&len), sizeof(len));

	// write the actual string data:
	file->write(str.c_str(), len);
}

std::string TileMap::readString(std::ifstream* file)
{
	// this probably isn't the optimal way to do it, but whatever
	std::string str;

	// get the length
	unsigned len;
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

void TileMap::changeTileFromMousePosition(int x, int y, int value)
{
	int realX, realY;
	realX = x / m_tileSize.x;
	realY = y / m_tileSize.y;
	if (realX < 0 || realX >= m_mapSize.x || realY < 0 || realY >= m_mapSize.y)
		return;
	changeTile(realX, realY, value);

}

void TileMap::changeTilesFromRectangle(sf::FloatRect rect, int value)
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

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

void TileMap::updateVertices(unsigned int x, unsigned int y)
{
	// get the current tile number
	int tileNumber = m_tiles[x + y * m_mapSize.x];

	// find its position in the tileset texture
	int tx = tileNumber % (m_tileset.getSize().x / m_tileSize.x);
	int ty = tileNumber / (m_tileset.getSize().x / m_tileSize.x);

	// get a pointer to the current tile's quad
	sf::Vertex* quad = &m_vertices[(x + y * m_mapSize.x) * 4];

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(tx * m_tileSize.x, ty * m_tileSize.y);
	quad[1].texCoords = sf::Vector2f((tx + 1) * m_tileSize.x, ty * m_tileSize.y);
	quad[2].texCoords = sf::Vector2f((tx + 1) * m_tileSize.x, (ty + 1) * m_tileSize.y);
	quad[3].texCoords = sf::Vector2f(tx * m_tileSize.x, (ty + 1) * m_tileSize.y);
}