#pragma once
#include "gameFIeld.h"
#include "RenderField.h"

class bigField : public sf::Drawable
{
public:
	bigField(sf::Vector2f windowSize);

	bigField(const bigField& other);

	bool placeSymbol(sf::Vector2f cursorCoords, State symbol);
	
	bool placeSymbol(sf::Vector2i fieldPosition, sf::Vector2i cellPosition, State symbol);

	bool isGameOver();

	void clear();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	~bigField();

private:
	GameField** boardsLogic;
	renderField** boardsRender;
	sf::Vector2f windowSize;
	sf::Vector2u boardSize;
	double gap = 20;
	bool gameBegin;

	void checkWin(GameField& field);

	void changeBoardsActivity(sf::Vector2i from, sf::Vector2i to);

};