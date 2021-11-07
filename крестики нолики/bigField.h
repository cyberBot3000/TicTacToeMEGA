#pragma once
#include "gameFIeld.h"
#include "RenderField.h"

class bigField : public sf::Drawable
{
public:
	bigField(sf::Vector2f windowSize);

	bigField(const bigField& other);

	void placeSymbol(sf::Vector2f cursorCoords, State symbol);

	void placeSymbol(sf::Vector2i cellPosition, GameField* activeField, State symbol);

	bool isGameOver();

	void clear();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	~bigField();

private:
	GameField** boardsLogic;
	GameField* activeField;
	renderField** boardsRender;
	renderField* activeRenderField;
	sf::Vector2f windowSize;
	sf::Vector2u boardSize;
	double gap = 20;
	bool gameBegin;

	void checkWin(GameField& field);

	void changeBoardsActivity(sf::Vector2i boardPosition);

};