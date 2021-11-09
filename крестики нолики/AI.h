#pragma once
#include "gameFIeld.h"

class AI
{
public:
	AI()
	{
		bigField = new GameField * [3];
		for (int i = 0; i < 3; i++)
		{
			bigField[i] = new GameField[3];
		}
	}
private:
	GameField** bigField;
};

