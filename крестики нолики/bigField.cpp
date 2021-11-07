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
	activeRenderField = nullptr;
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
	this->activeRenderField = new renderField(*other.activeRenderField);
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
void bigField::placeSymbol(sf::Vector2f cursorCoords, State symbol)
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
					activeField = &boardsLogic[newIconPos.x][newIconPos.y];
					activeRenderField = &boardsRender[newIconPos.x][newIconPos.y];
					if (activeField->noEmptyCells())
						activeField = nullptr;
					changeBoardsActivity(newIconPos);
				}
			}
		}
	}
}
void bigField::placeSymbol(sf::Vector2i cellPosition, GameField* activeField, State symbol)
{
	if (activeField->isActive())
	{
		sf::Vector2i newIconPos = cellPosition;
		if ((newIconPos != sf::Vector2i(-1, -1)) && (activeField->getSymbol(newIconPos.x, newIconPos.y) == EMPTY))
		{
			activeField->setSymbol(symbol, newIconPos.x, newIconPos.y);
			checkWin(*activeField);
			activeField = &boardsLogic[newIconPos.x][newIconPos.y];
			if (activeField->noEmptyCells())
				activeField = nullptr;
			changeBoardsActivity(newIconPos);
		}
	}
}
bool bigField::isGameOver()
{
	bool result = true;
	for (int i = 0; i < 3 && result; i++)
	{
		for (int j = 0; j < 3 && result; j++)
		{
			result *= (boardsLogic[i][j].getWinnerSymbol() != EMPTY && boardsLogic[i][j].noEmptyCells());
			result *= boardsLogic[i][j].noEmptyCells() == false;
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
	if (boardPosition.x >= 0 && boardPosition.y >= 0 && boardPosition.x < 3 && boardPosition.y < 3)
	{
		bool currentFieldActivity = true;
		if (boardsLogic[boardPosition.x][boardPosition.y].noEmptyCells())
		{
			currentFieldActivity = false;
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				boardsLogic[i][j].setActivity(!currentFieldActivity);
				if (i == boardPosition.x && j == boardPosition.y)
					boardsLogic[i][j].setActivity(currentFieldActivity);
				boardsRender[i][j].update();
			}
		}
	}
}