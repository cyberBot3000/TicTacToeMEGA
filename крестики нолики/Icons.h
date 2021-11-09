#pragma once
#include <SFML/Graphics.hpp>

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