#include <SFML/Graphics.hpp>

#include "Minesweeper/Minesweeper.h"

sf::Sprite GetTile(const sf::Texture& texture, Minesweeper::State gameState, const Minesweeper::Tile& tile, float x, float y)
{
	constexpr uint8_t spriteSize = 32;
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(x * spriteSize, y * spriteSize);

	switch (tile.type)
	{
		case Minesweeper::TileType::Ground:
			sprite.setTextureRect({ spriteSize * 12, 0, spriteSize, spriteSize });
			break;

		case Minesweeper::TileType::Safe:
			if (tile.numberOfMines > 0)
			{
				const uint8_t tileNumber = tile.numberOfMines - 1;
				sprite.setTextureRect({ spriteSize * tileNumber, 0, spriteSize, spriteSize });
			}
			else
			{
				sprite.setTextureRect({ spriteSize * 10, 0, spriteSize, spriteSize });
			}
			break;

		case Minesweeper::TileType::Mine:
			if (gameState == Minesweeper::State::Playing)
				sprite.setTextureRect({ spriteSize * 12, 0, spriteSize, spriteSize });
			else
				sprite.setTextureRect({ spriteSize * 8, 0, spriteSize, spriteSize });
			break;

		case Minesweeper::TileType::SelectedMine:
			sprite.setTextureRect({ spriteSize * 11, 0, spriteSize, spriteSize });
			break;

		default: break;
	}

	if (tile.flagged && gameState != Minesweeper::State::GameOver) sprite.setTextureRect({ spriteSize * 9, 0, spriteSize, spriteSize });

	return sprite;
}

int main()
{
	auto window = sf::RenderWindow(sf::VideoMode(500, 500), "Minesweeper", sf::Style::Close);
	auto view = sf::View({ 0, 0 }, sf::Vector2f(window.getSize()));

	sf::Texture assets;
	assets.loadFromFile("assets/Tiles.png");

	constexpr uint32_t sizeX      = 30;
	constexpr uint32_t sizeY      = 16;
	constexpr uint16_t cellSize   = 32;
	constexpr uint32_t numOfMines = 99;

	Minesweeper::Minesweeper game;
	Minesweeper::State gameState = game.CreateField(sizeX, sizeY, cellSize);

	window.setSize({ sizeX * cellSize, sizeY * cellSize });

	while (window.isOpen())
	{
		auto windowMousePosition = sf::Mouse::getPosition(window);
		auto mousePosition = window.mapPixelToCoords(windowMousePosition);

		for (auto event = sf::Event(); window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				view.setSize(event.size.width, event.size.height);
				view.setCenter(event.size.width / 2, event.size.height / 2);
				window.setView(view);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					gameState = game.CreateField(sizeX, sizeY, cellSize);
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (gameState == Minesweeper::State::Started)
					{
						gameState = game.SelectInitialTile(numOfMines, mousePosition.x, mousePosition.y);
					}

					if (gameState == Minesweeper::State::Playing)
					{
						gameState = game.SelectTile(mousePosition.x, mousePosition.y);
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					if (gameState == Minesweeper::State::Playing)
					{
						game.SetFlag(mousePosition.x, mousePosition.y);
					}
				}
			}
		}

		window.clear();

		const auto& tiles = game.GetTiles();
		for (uint32_t x = 0; x < sizeX; x++)
		{
			for (uint32_t y = 0; y < sizeY; y++)
			{
				auto& tile = tiles[x + y * sizeX];

				window.draw(GetTile(assets, gameState, tile, x, y));
			}
		}

		window.display();
	}
}
