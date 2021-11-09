#include "Icons.h"
Cross::Cross()
{
	t_cross.loadFromFile("cross.png");
	cross.setTexture(t_cross);
	cross.setScale(size.x / t_cross.getSize().x, size.y / t_cross.getSize().y);
}
Cross::Cross(sf::Vector2f position, sf::Vector2f size)
{
	this->position = position;
	this->size = size;
	cross.setPosition(position);
	t_cross.loadFromFile("cross.png");
	cross.setTexture(t_cross);
	cross.setScale(size.x / t_cross.getSize().x, size.y / t_cross.getSize().y);
}
void Cross::setPosition(sf::Vector2f position)
{
	this->position = position;
	cross.setPosition(position);
}
sf::Vector2f Cross::getPosition()
{
	return position;
}
void Cross::setSize(sf::Vector2f size)
{
	this->size = size;
}
void Cross::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(cross);
}

Zero::Zero()
{
	t_zero.loadFromFile("zero.png");
	zero.setTexture(t_zero);
	zero.setScale(size.x / t_zero.getSize().x, size.y / t_zero.getSize().y);
}
Zero::Zero(sf::Vector2f position, sf::Vector2f size)
{
	this->position = position;
	this->size = size;
	zero.setPosition(position);
	t_zero.loadFromFile("zero.png");
	zero.setTexture(t_zero);
	zero.setScale(size.x / t_zero.getSize().x, size.y / t_zero.getSize().y);
}
void Zero::setPosition(sf::Vector2f position)
{
	this->position = position;
	zero.setPosition(position);
}
sf::Vector2f Zero::getPosition()
{
	return position;
}
void Zero::setSize(sf::Vector2f size)
{
	this->size = size;
}
void Zero::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(zero);
}

Empty::Empty()
{

}
void Empty::setPosition(sf::Vector2f position)
{
	this->position = position;
}
sf::Vector2f Empty::getPosition()
{
	return position;
}
void Empty::setSize(sf::Vector2f size)
{
	this->size = size;
}
void Empty::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	return;
}