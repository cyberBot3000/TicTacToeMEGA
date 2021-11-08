#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include "bigField.h"

int main()
{
	srand(time(NULL));
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
	sf::Vector2i lastTurn = sf::Vector2i(0, 0);
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
					//gameOver = false;
				}
			}
			else
			{
				button.setOutlineColor(sf::Color::Black);
			}
		}
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !gameOver)
		{
			if (!mouseButtonHold)
			{
				mouseButtonHold = true;*/
		if (!gameOver) {
			sf::Vector2i newTurn = sf::Vector2i(rand() % 3, rand() % 3);
			if (nextTurn && game.placeSymbol(lastTurn, newTurn, CROSS) == true) {
				nextTurn = !nextTurn;
				lastTurn = newTurn;
			}
			else if (!nextTurn && game.placeSymbol(lastTurn, newTurn, ZERO) == true) {
				nextTurn = !nextTurn;
				lastTurn = newTurn;
			}
			else {
				lastTurn = sf::Vector2i(rand() % 3, rand() % 3);
			}
			if (game.isGameOver())
			{
				gameOver = true;
			}
		}
			/*}
		}
		else
		{
			mouseButtonHold = false;
		}*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			game.clear();
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