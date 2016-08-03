#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"

class GUI : public sf::Transformable, public sf::Drawable
{
public:
	GUI(unsigned width, unsigned height);
	~GUI();
	void handleMouseClick(float x, float y, int button);
	void resize(unsigned int width, unsigned int height);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape m_borderShapes[2];
	std::vector<Button*> m_buttons;
};

#endif