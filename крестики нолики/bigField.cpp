#include "bigField.h"

bigField::bigField()
{
	sf::Vector2f windowSize = sf::Vector2f(800, 800);
	boardsLogic = new GameField * [3];
	boardsRender = new renderField * [3];
	for (int i = 0; i < 3; i++)
	{
		boardsLogic[i] = new GameField[3];
		boardsRender[i] = new renderField[3];
	}
	this->windowSize = windowSize;
	gameBegin = true;
	boardSize = sf::Vector2u(windowSize.x / 3 - gap / 1.55, windowSize.y / 3 - gap / 1.55);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			boardsRender[i][j].setGameField(&boardsLogic[i][j]);
			boardsRender[i][j].setSize(boardSize);
			boardsRender[i][j].setPosition(sf::Vector2f(boardSize.x * i + gap * i, boardSize.y * j + gap * j));
			boardsLogic[i][j].setActivity(true);
		}
	}
}

bigField::bigField(sf::Vector2f windowSize)
{
	boardsLogic = new GameField * [3];
	boardsRender = new renderField * [3];
	for (int i = 0; i < 3; i++)
	{
		boardsLogic[i] = new GameField[3];
		boardsRender[i] = new renderField[3];
	}
	this->windowSize = windowSize;
	gameBegin = true;
	boardSize = sf::Vector2u(windowSize.x / 3 - gap / 1.55, windowSize.y / 3 - gap / 1.55);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			boardsRender[i][j].setGameField(&boardsLogic[i][j]);
			boardsRender[i][j].setSize(boardSize);
			boardsRender[i][j].setPosition(sf::Vector2f(boardSize.x * i + gap * i, boardSize.y * j + gap * j));
			boardsLogic[i][j].setActivity(true);
		}
	}
}
bigField::bigField(const bigField& other)
{
	this->boardsLogic = new GameField * [3];
	this->boardsRender = new renderField * [3];
	
	for (int i = 0; i < 3; i++)
	{
		this->boardsLogic[i] = new GameField[3];
		this->boardsRender[i] = new renderField[3];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->boardsLogic[i][j] = other.boardsLogic[i][j];
			this->boardsRender[i][j] = other.boardsRender[i][j];
		}
	}
	this->boardSize = other.boardSize;
	this->gameBegin = other.gameBegin;
	this->gap = other.gap;
	this->windowSize = other.windowSize;
}
 bool bigField::placeSymbol(sf::Vector2f cursorCoords, State symbol)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (boardsLogic[i][j].isActive())
			{
				sf::Vector2i newIconPos = boardsRender[i][j].coordsToCell(cursorCoords);
				if (inBoardRange(newIconPos) == false)
				{
					return false;
				}
				if (boardsLogic[i][j].getSymbol(newIconPos.x, newIconPos.y) == EMPTY)
				{
					boardsLogic[i][j].setSymbol(symbol, newIconPos.x, newIconPos.y);
					changeBoardsActivity(sf::Vector2i(i, j), newIconPos);
					return true;
				}
			}
		}
	}
	return false;
}
bool bigField::placeSymbol(sf::Vector2i fieldPosition, sf::Vector2i cellPosition, State symbol)
{
	if (inBoardRange(fieldPosition) == false) 
	{
		return false;
	}
	int x_a = fieldPosition.x;
	int y_a = fieldPosition.y;

	if (boardsLogic[x_a][y_a].isActive() == false)
	{
		return false;
	}

	sf::Vector2i newIconPos = cellPosition;
	if (inBoardRange(newIconPos) == false)
	{
		return false;
	}

	if ((boardsLogic[x_a][y_a].getSymbol(newIconPos.x, newIconPos.y) == EMPTY))
	{
		boardsLogic[x_a][y_a].setSymbol(symbol, newIconPos.x, newIconPos.y);
		changeBoardsActivity(fieldPosition, newIconPos);
		return true;
	}
	return false;
}
bool bigField::isWin(State symbol)
{
	for (int i = 0; i < 3; i++)
	{
		bool row = true, column = true, diagonal1 = true, diagonal2 = true;
		for (int j = 0; j < 3; j++)
		{
			row *= (boardsLogic[i][j].getWinnerSymbol() == symbol);
			column *= (boardsLogic[j][i].getWinnerSymbol() == symbol);
			diagonal1 *= (boardsLogic[j][j].getWinnerSymbol() == symbol);
			diagonal2 *= (boardsLogic[3 - 1 - j][j].getWinnerSymbol() == symbol);
		}
		if (row || column || diagonal1 || diagonal2)
		{
			return true;
		}
	}
	return false;
}

bool bigField::noEmptyFields()
{
	bool result = true;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result *= boardsLogic[i][j].getWinnerSymbol() != EMPTY || boardsLogic[i][j].noEmptyCells();
		}
	}
	return result;
}

void bigField::clear()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			boardsLogic[i][j].clear();
			boardsLogic[i][j].setActivity(true);
		}
	}
}
void bigField::update() 
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			boardsRender[i][j].update();
		}
	}
}
void bigField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			target.draw(boardsRender[i][j]);
		}
	}
}
bigField::~bigField()
{
	for (int i = 0; i < 3; i++)
	{
		delete[] boardsLogic[i];
		delete[] boardsRender[i];
	}
	delete[] boardsLogic;
	delete[] boardsRender;
}
void bigField::checkWin(GameField& field)
{
	if (field.getWinnerSymbol() != EMPTY)
		return;
	if (field.isWin(CROSS))
	{
		return;
	}
	else if (field.isWin(ZERO))
	{
		return;
	}
	else if (field.noEmptyCells())
	{
		return;
	}
	return;
}
void bigField::changeBoardsActivity(sf::Vector2i from, sf::Vector2i to)
{
	if (to.x < 0 && to.y < 0 && to.x >= 3 && to.y >= 3)
	{
		return;
	}
	bool currentFieldActivity = true;
	checkWin(boardsLogic[to.x][to.y]);
	if (boardsLogic[to.x][to.y].isGameOver())
	{
		currentFieldActivity = false;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			
			boardsLogic[i][j].setActivity(!currentFieldActivity * !boardsLogic[i][j].isGameOver());
			if (i == to.x && j == to.y) 
			{
				boardsLogic[i][j].setActivity(currentFieldActivity);
			}
		}
	}
}

bool bigField::inBoardRange(sf::Vector2i position)
{
	if (position.x < 0 || position.x > 2 || position.y < 0 || position.y > 2)
		return false;
	return true;
}
