#pragma once
#include "enums.h"
#include <vector>
class GameField
{
public:
	GameField();

	void setCellsColumn(int column = 3);

	int getCellsCol();

	bool isWin(State currentSymbol);

	bool noEmptyCells();

	void setSymbol(State symbol, int x, int y);

	void removeSymbol(int x, int y);

	State getSymbol(int x, int y);

	State getWinnerSymbol();

	void setActivity(bool isActive);

	bool isActive();

	void clear();

private:
	std::vector< std::vector<State> > cells;
	State winnerSymbol;
	bool activity;
};