#include "RenderField.h"
renderField::renderField()
{

}
renderField::renderField(GameField* fieldToDraw)
{
	field = fieldToDraw;
	fieldSize = sf::Vector2f(600, 600);
	cellSize = sf::Vector2f(fieldSize.x / (double)field->getCellsCol(), fieldSize.y / (double)field->getCellsCol());

	icons.resize(field->getCellsCol());
	for (int i = 0; i < field->getCellsCol(); i++)
	{
		icons[i].resize(field->getCellsCol(), NULL);
	}

	verticalBorders.resize(field->getCellsCol() + 1);
	horisontalBorders.resize(field->getCellsCol() + 1);
	makeVertialBorders();
	makeHorisontalBorders();
}
void renderField::setGameField(GameField* fieldToDraw)
{
	field = fieldToDraw;
	fieldSize = sf::Vector2f(600, 600);
	cellSize = sf::Vector2f(fieldSize.x / (double)field->getCellsCol(), fieldSize.y / (double)field->getCellsCol());

	icons.resize(field->getCellsCol());
	for (int i = 0; i < field->getCellsCol(); i++)
	{
		icons[i].resize(field->getCellsCol(), NULL);
	}

	verticalBorders.resize(field->getCellsCol() + 1);
	horisontalBorders.resize(field->getCellsCol() + 1);
	makeVertialBorders();
	makeHorisontalBorders();
}
void renderField::setSize(sf::Vector2u fieldSize)
{
	this->fieldSize = sf::Vector2f(fieldSize.x, fieldSize.y);
	cellSize = sf::Vector2f(fieldSize.x / (double)field->getCellsCol(), fieldSize.y / (double)field->getCellsCol());
	makeVertialBorders();
	makeHorisontalBorders();
	update();
}
void renderField::setPosition(sf::Vector2f position)
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
sf::Vector2i renderField::coordsToCell(sf::Vector2f cursorPosition)
{
	cursorPosition -= globalPosition;
	if ((cursorPosition.x <= 0 || cursorPosition.y <= 0) ||
		(cursorPosition.x >= fieldSize.x || cursorPosition.y >= fieldSize.y))
	{
		return sf::Vector2i(-1, -1);
	}
	return sf::Vector2i(cursorPosition.x / cellSize.x, cursorPosition.y / cellSize.y);
}
void renderField::update()
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
				if (icons[i][j] != NULL) 
				{
					delete icons[i][j];
				}
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
void renderField::draw(sf::RenderTarget& target, sf::RenderStates states) const
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
renderField::~renderField()
{
	for (int i = 0; i < field->getCellsCol(); i++)
	{
		for (int j = 0; j < field->getCellsCol(); j++)
		{
			delete icons[i][j];
		}
	}
}

renderField::Cross::Cross()
{
	t_cross.loadFromFile("cross.png");
	cross.setTexture(t_cross);
	cross.setScale(size.x / t_cross.getSize().x, size.y / t_cross.getSize().y);
}
renderField::Cross::Cross(sf::Vector2f position, sf::Vector2f size)
{
	this->position = position;
	this->size = size;
	cross.setPosition(position);
	t_cross.loadFromFile("cross.png");
	cross.setTexture(t_cross);
	cross.setScale(size.x / t_cross.getSize().x, size.y / t_cross.getSize().y);
}
void renderField::Cross::setPosition(sf::Vector2f position)
{
	this->position = position;
	cross.setPosition(position);
}
sf::Vector2f renderField::Cross::getPosition() 
{
	return position;
}
void renderField::Cross::setSize(sf::Vector2f size) 
{
	this->size = size;
}
void renderField::Cross::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(cross);
}

renderField::Zero::Zero()
{
	t_zero.loadFromFile("zero.png");
	zero.setTexture(t_zero);
	zero.setScale(size.x / t_zero.getSize().x, size.y / t_zero.getSize().y);
}
renderField::Zero::Zero(sf::Vector2f position, sf::Vector2f size)
{
	this->position = position;
	this->size = size;
	zero.setPosition(position);
	t_zero.loadFromFile("zero.png");
	zero.setTexture(t_zero);
	zero.setScale(size.x / t_zero.getSize().x, size.y / t_zero.getSize().y);
}
void renderField::Zero::setPosition(sf::Vector2f position)
{
	this->position = position;
	zero.setPosition(position);
}
sf::Vector2f renderField::Zero::getPosition()
{
	return position;
}
void renderField::Zero::setSize(sf::Vector2f size)
{
	this->size = size;
}
void renderField::Zero::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(zero);
}

renderField::Empty::Empty()
{

}
void renderField::Empty::setPosition(sf::Vector2f position)
{
	this->position = position;
}
sf::Vector2f renderField::Empty::getPosition()
{
	return position;
}
void renderField::Empty::setSize(sf::Vector2f size)
{
	this->size = size;
}
void renderField::Empty::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	return;
}

void renderField::makeHorisontalBorders()
{
	for (int i = 0; i < horisontalBorders.size(); i++)
	{
		horisontalBorders[i].setFillColor(sf::Color::Black);
		horisontalBorders[i].setSize(sf::Vector2f(fieldSize.x, 1));
		sf::Vector2f position = sf::Vector2f(0, cellSize.y * i);
		horisontalBorders[i].setPosition(position);
	}
}
void renderField::makeVertialBorders()
{
	for (int i = 0; i < verticalBorders.size(); i++)
	{
		verticalBorders[i].setFillColor(sf::Color::Black);
		verticalBorders[i].setSize(sf::Vector2f(1, fieldSize.y));
		sf::Vector2f position = sf::Vector2f(cellSize.x * i, 0);
		verticalBorders[i].setPosition(position);
	}
}