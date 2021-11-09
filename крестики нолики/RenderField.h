#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "gameFIeld.h"
#include "Icons.h"

class renderField : public sf::Drawable
{
public:
	renderField();

	renderField(GameField* fieldToDraw);

	void setGameField(GameField* fieldToDraw);

	void setSize(sf::Vector2u fieldSize);

	void setPosition(sf::Vector2f position);

	sf::Vector2i coordsToCell(sf::Vector2f cursorPosition);

	void update();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	~renderField();

private:

	void makeHorisontalBorders();

	void makeVertialBorders();

	sf::Vector2f globalPosition;
	GameField* field;
	std::vector<std::vector<Icon*> > icons;
	Icon* winnerIcon;
	sf::Vector2f fieldSize;
	sf::Vector2f cellSize;
	std::vector<sf::RectangleShape> verticalBorders;
	std::vector<sf::RectangleShape> horisontalBorders;

};