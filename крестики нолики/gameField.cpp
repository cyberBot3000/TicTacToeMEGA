#pragma once
#include "gameFIeld.h"

GameField::GameField() 
{
	cells.resize(3);
	for (int i = 0; i < cells.size(); i++)
	{
		cells[i].resize(3);
	}
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[i].size(); j++)
		{
			cells[i][j] = EMPTY;
		}
	}
	winnerSymbol = EMPTY;
	activity = true;
	gameOver = false;
}
void GameField::setCellsColumn(int column = 3)
{
	cells.resize(column);
	for (int i = 0; i < cells.size(); i++)
	{
		cells[i].resize(column);
	}
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[i].size(); j++)
		{
			cells[i][j] = EMPTY;
		}
	}
}
int GameField::getCellsCol()
{
	return cells.size();
}
bool GameField::isWin(State currentSymbol)
{
	for (int i = 0; i < cells.size(); i++)
	{
		bool row = true, column = true, diagonal1 = true, diagonal2 = true;
		for (int j = 0; j < cells[i].size(); j++)
		{
			row *= (cells[i][j] == currentSymbol);
			column *= (cells[j][i] == currentSymbol);
			diagonal1 *= (cells[j][j] == currentSymbol);
			diagonal2 *= (cells[cells.size() - 1 - j][j] == currentSymbol);
		}
		if (row || column || diagonal1 || diagonal2)
		{
			winnerSymbol = currentSymbol;
			gameOver = true;
			return true;
		}
	}
	return false;
}
bool GameField::noEmptyCells()
{
	bool result = 1;
	for (int i = 0; i < cells.size() && result; i++)
	{
		for (int j = 0; j < cells[i].size() && result; j++)
		{
			result *= (cells[i][j] != EMPTY);
		}
	}
	if (!gameOver) 
	{
		gameOver = result;
	}
	return result;
}
bool GameField::isGameOver()
{
	return gameOver;
}
void GameField::setSymbol(State symbol, int x, int y)
{
	int a = 0;
	cells[x][y] = symbol;
}
void GameField::removeSymbol(int x, int y)
{
	cells[x][y] = EMPTY;
}
State GameField::getSymbol(int x, int y)
{
	State result = static_cast<State> (cells[x][y]);
	return result;
}
State GameField::getWinnerSymbol()
{
	return winnerSymbol;
}
void GameField::setActivity(bool isActive)
{
	activity = isActive;
}
bool GameField::isActive()
{
	return activity;
}
void GameField::clear()
{
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[i].size(); j++)
		{
			cells[i][j] = EMPTY;
		}
	}
	winnerSymbol = EMPTY;
	gameOver = false;
}