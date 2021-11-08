#include "bigField.h"
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
	activeField = nullptr;
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
	this->activeField = new GameField(*other.activeField);
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
				if ((newIconPos != sf::Vector2i(-1, -1)) && (boardsLogic[i][j].getSymbol(newIconPos.x, newIconPos.y) == EMPTY))
				{
					boardsLogic[i][j].setSymbol(symbol, newIconPos.x, newIconPos.y);
					changeBoardsActivity(newIconPos);
					return true;
				}
			}
		}
	}
	return false;
}
void bigField::placeSymbol(sf::Vector2i cellPosition, GameField* activeField, State symbol)
{
	if (activeField->isActive())
	{
		sf::Vector2i newIconPos = cellPosition;
		if ((newIconPos != sf::Vector2i(-1, -1)) && (activeField->getSymbol(newIconPos.x, newIconPos.y) == EMPTY))
		{
			activeField->setSymbol(symbol, newIconPos.x, newIconPos.y);
			changeBoardsActivity(newIconPos);
		}
	}
}
bool bigField::placeSymbol(sf::Vector2i fieldPosition, sf::Vector2i cellPosition, State symbol)
{
	if (fieldPosition.x < 0 || fieldPosition.x > 2 || fieldPosition.y < 0 || fieldPosition.y > 2)
		return false;
	int x_a = fieldPosition.x;
	int y_a = fieldPosition.y;
	if (boardsLogic[x_a][y_a].isActive())
	{
		sf::Vector2i newIconPos = cellPosition;
		if (newIconPos.x < 0 || newIconPos.x > 2 || newIconPos.y < 0 || newIconPos.y > 2)
			return false;

		if ((boardsLogic[x_a][y_a].getSymbol(newIconPos.x, newIconPos.y) == EMPTY))
		{
			boardsLogic[x_a][y_a].setSymbol(symbol, newIconPos.x, newIconPos.y);
			changeBoardsActivity(newIconPos);
			return true;
		}
	}
	return false;
}
bool bigField::isGameOver()
{
	bool result = true;
	for (int i = 0; i < 3 && result; i++)
	{
		for (int j = 0; j < 3 && result; j++)
		{
			checkWin(boardsLogic[i][j]);
			result *= (boardsLogic[i][j].getWinnerSymbol() != EMPTY) || (boardsLogic[i][j].getWinnerSymbol() == EMPTY && boardsLogic[i][j].noEmptyCells());
		}
	}
	if(result == true)
		return result;
	result = true;
	for (int i = 0; i < 3 && result; i++)
	{
		for (int j = 0; j < 3 && result; j++)
		{
			result *= boardsLogic[i][j].noEmptyCells();
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
void bigField::changeBoardsActivity(sf::Vector2i boardPosition)
{
	if (boardPosition.x < 0 && boardPosition.y < 0 && boardPosition.x >= 3 && boardPosition.y >= 3)
	{
		return;
	}
	bool currentFieldActivity = true;
	if (boardsLogic[boardPosition.x][boardPosition.y].noEmptyCells())
	{
		currentFieldActivity = false;
		this->activeField = nullptr;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			boardsLogic[i][j].setActivity(!currentFieldActivity);
			if (i == boardPosition.x && j == boardPosition.y) 
			{
				boardsLogic[i][j].setActivity(currentFieldActivity);
				this->activeField = &boardsLogic[i][j];
			}
			boardsRender[i][j].update();
		}
	}
}