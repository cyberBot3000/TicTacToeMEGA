#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

enum State
{
	EMPTY,
	CROSS,
	ZERO,
};
enum turn
{
	USER_TURN = false,
	AI_TURN = true,
};

class GameField
{
public:
	GameField()
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
		// LOL hui lol
	}
	void setCellsColumn(int column = 3)
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
	int getCellsCol()
	{
		return cells.size();
	}
	bool isWin(State currentSymbol)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			bool row = true, column = true, diagonal1 = true, diagonal2 = true;
			for (int j = 0; j < cells[i].size(); j++)
			{
				row *= (cells[i][j] == currentSymbol);
				column *= (cells[j][i] == currentSymbol);
				diagonal1 *= (cells[j][j] == currentSymbol);
				diagonal2 *= (cells[cells.size()-1 - j][j] == currentSymbol);
			}
			if (row || column || diagonal1 || diagonal2) 
			{
				winnerSymbol = currentSymbol;
				return true;
			}
		}
		return false;
	}
	bool noEmptyCells() 
	{
		bool result = 1;
		for (int i = 0; i < cells.size() && result; i++)
		{
			for (int j = 0; j < cells[i].size() && result; j++)
			{
				result *= (cells[i][j] != EMPTY);
			}
		}
		
		return result;
	}
	void setSymbol(State symbol, int x, int y)
	{
		int a = 0;
		cells[x][y] = symbol;
	}
	void removeSymbol(int x, int y)
	{
		cells[x][y] = EMPTY;
	}
	State getSymbol(int x, int y)
	{
		State result = static_cast<State> (cells[x][y]);
		return result;
	}
	State getWinnerSymbol()
	{
		return winnerSymbol;
	}
	void setActivity(bool isActive)
	{
		activity = isActive;
	}
	bool isActive()
	{
		return activity;
	}
	void clear()
	{
		for (int i = 0; i < cells.size(); i++)
		{
			for (int j = 0; j < cells[i].size(); j++)
			{
				cells[i][j] = EMPTY;
				winnerSymbol = EMPTY;
			}
		}
	}
private:
	std::vector< std::vector<State> > cells;
	State winnerSymbol;
	bool activity;
};

class SimpleFieldAI
{
public:
	SimpleFieldAI()
	{
	}
	static sf::Vector2i getNewTurn(GameField& field)
	{
		GameField field_temp = field;
		sf::Vector2i newPosition = sf::Vector2i(-1, -1);
		int bestScore = INT_MIN;
		for (int i = 0; i < field.getCellsCol(); i++)
		{
			for (int j = 0; j < field.getCellsCol(); j++)
			{
				if (field_temp.getSymbol(i, j) == EMPTY)
				{
					field_temp.setSymbol(AI_Symbol, i, j);
					int score = minimax(field_temp, 0, USER_TURN);
					field_temp.removeSymbol(i, j);
					if (score > bestScore) 
					{
						bestScore = score;
						newPosition = sf::Vector2i(i, j);
					}
				}
			}
		}
		return newPosition;
	}
	static double getLoseChance(GameField& field, turn nextTurn)
	{
		wins = loses = draws = 0;
		GameField field_temp = field;
		for (int i = 0; i < field.getCellsCol(); i++)
		{
			for (int j = 0; j < field.getCellsCol(); j++)
			{
				if (field_temp.getSymbol(i, j) == EMPTY)
				{
					field_temp.setSymbol(AI_Symbol, i, j);
					minimax(field_temp, 0, nextTurn);
					field_temp.removeSymbol(i, j);
				}
			}
		}
		return (double)loses / (double)(loses + wins);
	}
private:
	static State AI_Symbol;
	static State userSymbol;
	static int wins;
	static int loses;
	static int draws;
	
	enum scores
	{
		WIN = 100,
		LOSE = -100,
		DRAW = 0,
	};
	static int minimax(GameField& field, int depth, turn currentTurn)
	{
		if (field.isWin(AI_Symbol))
		{
			loses++;
			return WIN / (depth + 1);
		}
		if (field.isWin(userSymbol))
		{
			wins++;
			return LOSE * (depth + 1);
		}
		if (field.noEmptyCells())
		{
			draws++;
			return DRAW;
		}
		int bestScore;
		if (currentTurn == AI_TURN)
		{
			// chose maximum score
			bestScore = INT_MIN;
			for (int i = 0; i < field.getCellsCol(); i++)
			{
				for (int j = 0; j < field.getCellsCol(); j++)
				{
					if (field.getSymbol(i, j) == EMPTY)
					{
						field.setSymbol(AI_Symbol, i, j);
						int score = minimax(field, depth + 1, USER_TURN);
						field.removeSymbol(i, j);
						bestScore = std::max(bestScore, score);
					}
				}
			}
		}
		else
		{
			// chose minimum score
			bestScore = INT_MAX;
			for (int i = 0; i < field.getCellsCol(); i++)
			{
				for (int j = 0; j < field.getCellsCol(); j++)
				{
					if (field.getSymbol(i, j) == EMPTY)
					{
						field.setSymbol(userSymbol, i, j);
						int score = minimax(field, depth + 1, AI_TURN);
						field.removeSymbol(i, j);
						bestScore = std::min(bestScore, score);
					}
				}
			}
		}
		return bestScore;
	}
};
int SimpleFieldAI::wins = 0;
int SimpleFieldAI::loses = 0;
int SimpleFieldAI::draws = 0;
State SimpleFieldAI::AI_Symbol = ZERO;
State SimpleFieldAI::userSymbol = CROSS;

class renderField : public sf::Drawable
{
public:
	renderField()
	{
		
	}
	renderField(GameField* fieldToDraw)
	{
		field = fieldToDraw;
		fieldSize = sf::Vector2f(600, 600);
		cellSize = sf::Vector2f(fieldSize.x / (double)field->getCellsCol(), fieldSize.y / (double)field->getCellsCol());

		icons.resize(field->getCellsCol());
		for (int i = 0; i < field->getCellsCol(); i++)
		{
			icons[i].resize(field->getCellsCol(), new Empty());
		}
		
		verticalBorders.resize(field->getCellsCol() + 1);
		horisontalBorders.resize(field->getCellsCol() + 1);
		makeVertialBorders();
		makeHorisontalBorders();
	}
	void setGameField(GameField* fieldToDraw)
	{
		field = fieldToDraw;
		fieldSize = sf::Vector2f(600, 600);
		cellSize = sf::Vector2f(fieldSize.x / (double)field->getCellsCol(), fieldSize.y / (double)field->getCellsCol());

		icons.resize(field->getCellsCol());
		for (int i = 0; i < field->getCellsCol(); i++)
		{
			icons[i].resize(field->getCellsCol(), new Empty());
		}

		verticalBorders.resize(field->getCellsCol() + 1);
		horisontalBorders.resize(field->getCellsCol() + 1);
		makeVertialBorders();
		makeHorisontalBorders();
	}
	void setSize(sf::Vector2u fieldSize)
	{
		this->fieldSize = sf::Vector2f(fieldSize.x, fieldSize.y);
		cellSize = sf::Vector2f(fieldSize.x / (double)field->getCellsCol(), fieldSize.y / (double)field->getCellsCol());
		makeVertialBorders();
		makeHorisontalBorders();
		update();
	}
	void setPosition(sf::Vector2f position)
	{
		globalPosition = position;
		for (int i = 0; i < field->getCellsCol(); i++)
		{
			for (int j = 0; j < field->getCellsCol(); j++)
			{
				icons[i][j]->setPosition(icons[i][j]->getPosition() + position);
			}
		}
		for (int i = 0; i < horisontalBorders.size(); i++)
		{
			horisontalBorders[i].setPosition(horisontalBorders[i].getPosition() + position);
			verticalBorders[i].setPosition(verticalBorders[i].getPosition() + position);
		}
	}
	
	sf::Vector2i coordsToCell(sf::Vector2f cursorPosition)
	{
		cursorPosition -= globalPosition;
		if ( (cursorPosition.x < 0 || cursorPosition.y < 0) ||
			 (cursorPosition.x > fieldSize.x || cursorPosition.y > fieldSize.y) )
		{
			return sf::Vector2i(-1, -1);
		}
		return sf::Vector2i(cursorPosition.x / cellSize.x, cursorPosition.y / cellSize.y);
	}
	void update()
	{
		for (int i = 0; i < field->getCellsCol(); i++)
		{
			for (int j = 0; j < field->getCellsCol(); j++)
			{
				State currentSymbol = field->getSymbol(i, j);
				switch (currentSymbol)
				{
				case CROSS:
					delete icons[i][j];
					icons[i][j] = new Cross(sf::Vector2f(i * cellSize.x, j * cellSize.y) + globalPosition, cellSize);
					break;
				case ZERO:
					delete icons[i][j];
					icons[i][j] = new Zero(sf::Vector2f(i * cellSize.x, j * cellSize.y) + globalPosition, cellSize);
					break;
				default:
					icons[i][j] = new Empty();
					break;
				}
			}
		}
		if (field->isActive())
		{
			for (int i = 0; i < horisontalBorders.size(); i++)
			{
				horisontalBorders[i].setOutlineThickness(1);
				verticalBorders[i].setOutlineThickness(1);
				horisontalBorders[i].setOutlineColor(sf::Color::Black);
				verticalBorders[i].setOutlineColor(sf::Color::Black);
			}
		}
		else
		{
			for (int i = 0; i < horisontalBorders.size(); i++)
			{
				horisontalBorders[i].setOutlineThickness(0);
				verticalBorders[i].setOutlineThickness(0);
				horisontalBorders[i].setOutlineColor(sf::Color::Transparent);
				verticalBorders[i].setOutlineColor(sf::Color::Transparent);
			}
		}
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (int i = 0; i < field->getCellsCol(); i++)
		{
			for (int j = 0; j < field->getCellsCol(); j++)
			{
				target.draw(*icons[i][j]);
			}
		}
		for (int i = 0; i < horisontalBorders.size(); i++)
		{
			target.draw(horisontalBorders[i]);
			target.draw(verticalBorders[i]);
		}
	}
	~renderField()
	{
		for (int i = 0; i < field->getCellsCol(); i++)
		{
			for (int j = 0; j < field->getCellsCol(); j++)
			{
				delete icons[i][j];
			}
		}
	}

private:
	class Icon : public sf::Drawable
	{
	public:
		virtual void setPosition(sf::Vector2f position) = 0;
		virtual sf::Vector2f getPosition() = 0;
		virtual void setSize(sf::Vector2f size) = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
		
	private:

	};

	class Cross : public Icon
	{
	public:
		Cross()
		{
			t_cross.loadFromFile("cross.png");
			cross.setTexture(t_cross);
			cross.setScale(size.x / t_cross.getSize().x, size.y / t_cross.getSize().y);
		}
		Cross(sf::Vector2f position, sf::Vector2f size)
		{
			this->position = position;
			this->size = size;
			cross.setPosition(position);
			t_cross.loadFromFile("cross.png");
			cross.setTexture(t_cross);
			cross.setScale(size.x / t_cross.getSize().x, size.y / t_cross.getSize().y);
		}
		virtual void setPosition(sf::Vector2f position) override
		{
			this->position = position;
			cross.setPosition(position);
		}
		virtual sf::Vector2f getPosition() override
		{
			return position;
		}
		virtual void setSize(sf::Vector2f size) override
		{
			this->size = size;
		}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(cross);
		}
	private:
		sf::Vector2f position;
		sf::Texture t_cross;
		sf::Sprite cross;
		sf::Vector2f size;
	};

	class Zero : public Icon
	{
	public:
		Zero()
		{
			t_zero.loadFromFile("zero.png");
			zero.setTexture(t_zero);
			zero.setScale(size.x / t_zero.getSize().x, size.y / t_zero.getSize().y);
		}
		Zero(sf::Vector2f position, sf::Vector2f size)
		{
			this->position = position;
			this->size = size;
			zero.setPosition(position);
			t_zero.loadFromFile("zero.png");
			zero.setTexture(t_zero);
			zero.setScale(size.x / t_zero.getSize().x, size.y / t_zero.getSize().y);
		}
		virtual void setPosition(sf::Vector2f position) override
		{
			this->position = position;
			zero.setPosition(position);
		}
		virtual sf::Vector2f getPosition() override
		{
			return position;
		}
		virtual void setSize(sf::Vector2f size) override
		{
			this->size = size;
		}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(zero);
		}
	private:
		sf::Vector2f position;
		sf::Texture t_zero;
		sf::Sprite zero;
		sf::Vector2f size;
	};
	
	class Empty : public Icon
	{
	public:

		Empty()
		{
			
		}
		virtual void setPosition(sf::Vector2f position) override
		{
			this->position = position;
		}
		virtual sf::Vector2f getPosition() override
		{
			return position;
		}
		virtual void setSize(sf::Vector2f size) override
		{
			this->size = size;
		}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			return;
		}
	private:
		sf::Vector2f position;
		sf::Vector2f size;
	};

	void makeHorisontalBorders()
	{
		for (int i = 0; i < horisontalBorders.size(); i++)
		{
			horisontalBorders[i].setFillColor(sf::Color::Black);
			horisontalBorders[i].setSize(sf::Vector2f(fieldSize.x, 1));
			sf::Vector2f position = sf::Vector2f(0, cellSize.y * i);
			horisontalBorders[i].setPosition(position);
		}
	}
	void makeVertialBorders()
	{
		for (int i = 0; i < verticalBorders.size(); i++)
		{
			verticalBorders[i].setFillColor(sf::Color::Black);
			verticalBorders[i].setSize(sf::Vector2f(1, fieldSize.y));
			sf::Vector2f position = sf::Vector2f(cellSize.x * i, 0);
			verticalBorders[i].setPosition(position);
		}
	}

	sf::Vector2f globalPosition;
	GameField* field;
	std::vector<std::vector<Icon*> > icons;
	sf::Vector2f fieldSize;
	sf::Vector2f cellSize;
	std::vector<sf::RectangleShape> verticalBorders;
	std::vector<sf::RectangleShape> horisontalBorders;

};

class bigField : public sf::Drawable
{
public:
	bigField (sf::Vector2f windowSize)
	{
		boardsLogic = new GameField*[3];
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
	bigField(const bigField& other)
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

	void placeSymbol(sf::Vector2f cursorCoords, State symbol)
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
	void placeSymbol(sf::Vector2i cellPosition, GameField *activeField, State symbol)
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

	void AImove()
	{
		if (activeField == nullptr)
		{
			sf::Vector2i bestField(-1, -1);
			double maxLoseChance = -1;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (boardsLogic[i][j].noEmptyCells() == false) 
					{
						double currentLoseChance = SimpleFieldAI::getLoseChance(boardsLogic[i][j], USER_TURN);
						if (currentLoseChance > maxLoseChance)
						{
							maxLoseChance = currentLoseChance;
							bestField = sf::Vector2i(i, j);
						}
					}
				}
			}
			sf::Vector2i bestPosition = SimpleFieldAI::getNewTurn(boardsLogic[bestField.x][bestField.y]);
			placeSymbol(bestPosition, &boardsLogic[bestField.x][bestField.y], ZERO);
			return;
		}
		sf::Vector2i newPosition = SimpleFieldAI::getNewTurn(*activeField);
		activeField->setSymbol(ZERO, newPosition.x, newPosition.y);
		if (activeField->isWin(ZERO))
		{
			activeField->removeSymbol(newPosition.x, newPosition.y);
			placeSymbol(newPosition, activeField, ZERO);
			return;
		}
		activeField->removeSymbol(newPosition.x, newPosition.y);
		double maxLoseChance = -1;
		sf::Vector2i bestPosition(-1, -1);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (activeField->getSymbol(i, j) == EMPTY)
				{
					double currentLoseChance = SimpleFieldAI::getLoseChance(boardsLogic[i][j], USER_TURN);
					if (currentLoseChance > maxLoseChance)
					{
						maxLoseChance = currentLoseChance;
						bestPosition = sf::Vector2i(i, j);
					}
				}
			}
		}
		placeSymbol(bestPosition, activeField, ZERO);
	}

	bool isGameOver()
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

	void clear()
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
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				target.draw(boardsRender[i][j]);
			}
		}
	}

	~bigField()
	{
		for (int i = 0; i < 3; i++)
		{
			delete[] boardsLogic[i];
			delete[] boardsRender[i];
		}
		delete[] boardsLogic;
		delete[] boardsRender;
	}

private:
	GameField** boardsLogic;
	GameField* activeField;
	renderField** boardsRender;
	renderField* activeRenderField;
	sf::Vector2f windowSize;
	sf::Vector2u boardSize;
	double gap = 20;
	bool gameBegin;
	void checkWin(GameField &field)
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
	void changeBoardsActivity(sf::Vector2i boardPosition)
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
					if(i == boardPosition.x && j == boardPosition.y)
						boardsLogic[i][j].setActivity(currentFieldActivity);
					boardsRender[i][j].update();
				}
			}
		}
	}
	

};



int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	sf::RectangleShape background;
	background.setFillColor(sf::Color::White);
	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	sf::Font font;
	font.loadFromFile("impact.ttf");
	sf::Text button;
	button.setFont(font);
	button.setString("Play Again");
	button.setCharacterSize(window.getSize().x * 0.10);
	button.setFillColor(sf::Color::White);
	button.setOutlineColor(sf::Color::Black);
	button.setOutlineThickness(2);
	button.setPosition(sf::Vector2f(window.getSize().x / 2 - button.getGlobalBounds().width / 2, window.getSize().y / 2 - button.getGlobalBounds().height / 2));
	

	bigField game(sf::Vector2f(window.getSize().x, window.getSize().y));
	bool mouseButtonHold = false;
	bool gameBegin = true;
	bool gameOver = false;
	bool nextTurn = true;
	while (window.isOpen())
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f cursorCoords = window.mapPixelToCoords(pixelPos);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (gameOver)
		{
			if (button.getGlobalBounds().contains(cursorCoords))
			{
				button.setOutlineColor(sf::Color(128, 128, 128));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					game.clear();
					gameOver = false;
				}
			}
			else
			{
				button.setOutlineColor(sf::Color::Black);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !gameOver)
		{
			if (!mouseButtonHold)
			{
				mouseButtonHold = true;
				if (nextTurn)
					game.placeSymbol(cursorCoords, CROSS);
				else 
					game.placeSymbol(cursorCoords, ZERO);
				nextTurn = !nextTurn;
				if (game.isGameOver())
				{
					gameOver = true;
				}
			}
		}
		else
		{
			mouseButtonHold = false;
		}
		
		window.clear();
		window.draw(background);
		window.draw(game);
		if (gameOver)
		{
			window.draw(button);
		}
		window.display();
	}
	return 0;
}