#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "gameFIeld.h"

class renderField : public sf::Drawable
{
public:
	renderField();

	renderField(GameField* fieldToDraw);

	void setGameField(GameField* fieldToDraw);

	void setSize(sf::Vector2u fieldSize);

	void setPosition(sf::Vector2f position);

	sf::Vector2i coordsToCell(sf::Vector2f cursorPosition);

	void update();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	~renderField();

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
		Cross();

		Cross(sf::Vector2f position, sf::Vector2f size);

		virtual void setPosition(sf::Vector2f position) override;

		virtual sf::Vector2f getPosition() override;

		virtual void setSize(sf::Vector2f size) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Vector2f position;
		sf::Texture t_cross;
		sf::Sprite cross;
		sf::Vector2f size;
	};

	class Zero : public Icon
	{
	public:
		Zero();

		Zero(sf::Vector2f position, sf::Vector2f size);

		virtual void setPosition(sf::Vector2f position) override;

		virtual sf::Vector2f getPosition() override;

		virtual void setSize(sf::Vector2f size) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::Vector2f position;
		sf::Texture t_zero;
		sf::Sprite zero;
		sf::Vector2f size;
	};

	class Empty : public Icon
	{
	public:

		Empty();

		virtual void setPosition(sf::Vector2f position) override;

		virtual sf::Vector2f getPosition() override;

		virtual void setSize(sf::Vector2f size) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::Vector2f position;
		sf::Vector2f size;
	};

	void makeHorisontalBorders();

	void makeVertialBorders();

	sf::Vector2f globalPosition;
	GameField* field;
	std::vector<std::vector<Icon*> > icons;
	sf::Vector2f fieldSize;
	sf::Vector2f cellSize;
	std::vector<sf::RectangleShape> verticalBorders;
	std::vector<sf::RectangleShape> horisontalBorders;

};