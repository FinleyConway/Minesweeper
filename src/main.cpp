#include <SFML/Graphics.hpp>

#include "Grid.h"

enum class TileType { Ground, Safe, Mine, Flag  };

struct Tile
{
	uint8_t numberOfMinds = 0;
	TileType type = TileType::Ground;
};

void PlaceMines(uint32_t numOfMines, Minesweeper::Grid<Tile>& grid, int32_t excludeX, int32_t excludeY, uint32_t excludeSize)
{
	const uint32_t width = grid.GetWidth();
	const uint32_t height = grid.GetHeight();

	std::vector<std::pair<uint32_t, uint32_t>> availableCells;
	availableCells.reserve(width * height);

	// get all non excluded tiles
	for (int32_t x = 0; x < width; x++)
	{
		for (int32_t y = 0; y < height; y++)
		{
			// skip all cells around an area
			if (std::abs(x - excludeX) <= excludeSize && std::abs(y - excludeY) <= excludeSize) continue;

			availableCells.emplace_back(x, y);
		}
	}

	// shuffles the tiles
	std::random_device rd;
	std::mt19937 gen(rd());
	std::ranges::shuffle(availableCells, gen);

	// place bombs in the random list of positions
	for (uint32_t i = 0; i < numOfMines; ++i)
	{
		auto [x, y] = availableCells[i];
		auto& tile = grid.Get(x, y);

		tile.type = TileType::Mine;
	}
}

void FloodPlain(Minesweeper::Grid<Tile>& grid, Tile& tile, int32_t tileX, int32_t tileY)
{
	uint8_t numOfMines = 0;

	// mark tile as safe
	tile.type = TileType::Safe;

	// get the number of surrounding bombs
	for (auto [x, y] : grid.GetSurrounding(tileX, tileY))
	{
		const auto& surroundingTile = grid.Get(x, y);

		if (surroundingTile.type == TileType::Mine) numOfMines++;
	}

	tile.numberOfMinds = numOfMines;

	// stop flood if there is a surrounding mine
	if (numOfMines > 0) return;

	// continue flood if there are no surrounding mines
	for (auto [x, y] : grid.GetSurrounding(tileX, tileY))
	{
		auto& surroundingTile = grid.Get(x, y);

		// only flood cells that have not been visited yet
		if (surroundingTile.type != TileType::Safe)
		{
			FloodPlain(grid, surroundingTile, x, y);
		}
	}
}

int main()
{
	auto window = sf::RenderWindow(sf::VideoMode(500, 500), "Minesweeper", sf::Style::Close);

	Minesweeper::Grid<Tile> grid;

	grid.Create(30, 16, 8);

	PlaceMines(99, grid, 15, 8, 1);
	FloodPlain(grid, grid.Get(15, 8), 15, 8);

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
				rect.setOutlineColor(sf::Color::Black);

				const auto tile = grid.Get(x, y);

				if (tile.type == TileType::Ground)
				{
					rect.setFillColor(sf::Color::Green);
				}
				else if (tile.type == TileType::Safe)
				{
					rect.setFillColor(sf::Color::Blue);
				}
				else if (tile.type == TileType::Mine)
				{
					rect.setFillColor(sf::Color::Black);
				}

				window.draw(rect);
			}
		}

		window.display();
	}
}