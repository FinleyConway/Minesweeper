#include <iostream>
#include <SFML/Graphics.hpp>

#include "Grid.h"

enum class TileType { Ground, Safe, Bomb, Flag  };

struct Tile
{
	uint8_t numberOfMinds = 0;
	TileType type = TileType::Ground;
};

int main()
{
	auto window = sf::RenderWindow(sf::VideoMode(500, 500), "Minesweeper", sf::Style::Close);

	Minesweeper::Grid<Tile> grid;

	grid.Create(30, 16, 8);

	for (uint8_t i = 0; i < 99; i++)
	{
		auto tile = Tile(0, TileType::Bomb);

		grid.Set(i, tile);
	}

	grid.Shuffle();

	while (window.isOpen())
	{
		for (auto event = sf::Event(); window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		for (int x = 0; x < 30; x++)
		{
			for (int y = 0; y < 16; y++)
			{
				sf::RectangleShape rect;
				rect.setSize({8, 8});
				rect.setPosition(x * 8, y * 8);

				const auto tile = grid.Get(x, y);

				if (tile.type == TileType::Ground)
				{
					rect.setFillColor(sf::Color::Green);
				}
				else if (tile.type == TileType::Bomb)
				{
					rect.setFillColor(sf::Color::Black);
				}

				window.draw(rect);
			}
		}

		window.display();
	}
}