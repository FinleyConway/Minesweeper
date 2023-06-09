#include "MineField.hpp"

namespace Minesweeper {

	/*
	* Easy = 8x8, 10 mines
	* Medium = 16x16, 40 mines
	* Hard = 30x16, 99 mines
	*/
	Difficulty m_Difficulty = Difficulty(Vector2Int(30, 16), 99);
	const uint32_t c_SpriteSize = 32;

	MineField::MineField(sf::RenderWindow& window, TextureManager& textureManager) :
		m_GridRenderer(m_Grid, textureManager),
		m_Grid(Grid<GridObject>(m_Difficulty.GridSize.X(), m_Difficulty.GridSize.Y(), c_SpriteSize, Vector2::Zero(), [](int32_t x, int32_t y, Grid<GridObject>& g) { return GridObject(x, y, g); }))
	{
		for (auto& tile : m_Grid.GetGrid())
		{
			tile->SetTexture(textureManager.GetTexture("Tile"));
		}

		window.setSize({ m_Difficulty.GridSize.X() * c_SpriteSize, m_Difficulty.GridSize.Y() * c_SpriteSize });
	}

	void MineField::PlayMinesweeper(sf::RenderWindow& window)
	{
		if (m_GameState == GameState::Start)
		{
			WaitForFirstAction(window);
		}
		else if (m_GameState == GameState::Playing)
		{
			PlayerChoice(window);
		}
		else if (m_GameState == GameState::GameOver)
		{

		}
	}

	void MineField::WaitForFirstAction(sf::RenderWindow& window)
	{
		// check if the left mouse button is pressed and the game has not yet started
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			while (sf::Mouse::isButtonPressed(sf::Mouse::Left));

			GridObject* tile = GetTileAtPosition(window);

			if (tile)
			{
				// place mines on the game grid, except for the clicked tile
				PlaceMines(tile->GetPosition());

				// initialize floodplain
				FloodPlain(*tile);

				m_GameState = GameState::Playing;
			}
		}
	}

	void MineField::PlaceMines(Vector2Int initPosition)
	{
		// initialize random number generator
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> randomNumberX(0, m_Grid.GetWidth() - 1);
		std::uniform_int_distribution<std::mt19937::result_type> randomNumberY(0, m_Grid.GetHeight() - 1);

		std::vector<Vector2Int> takenPositions;
		uint32_t bombsPlaced = 0;
		uint32_t totalTiles = m_Grid.GetWidth() * m_Grid.GetHeight();

		// keep placing bombs until the desired number is reached or there are no more available positions
		while (bombsPlaced < m_Difficulty.MinesAmount && takenPositions.size() < totalTiles)
		{
			// generate a random position on the game grid
			Vector2Int position(randomNumberX(rng), randomNumberY(rng));

			// check if the position has already been taken or if it's the initial position
			if (std::find(takenPositions.begin(), takenPositions.end(), position) != takenPositions.end() || position == initPosition)
			{
				continue;
			}

			// get the tile at the randomly generated position
			GridObject* tile = m_Grid.GetObject(position.X(), position.Y());
			if (tile)
			{
				// set the tile as a bomb and update its texture
				tile->SetTileType(TileType::Bomb);

				bombsPlaced++;
			}

			// add the position to the taken positions vector
			takenPositions.push_back(position);
		}
	}

	GridObject* MineField::GetTileAtPosition(sf::RenderWindow& window)
	{
		// get the position of the mouse click
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		Vector2 position(mousePosition.x, mousePosition.y);

		int32_t x, y;
		m_Grid.ConvertScreenToGrid(position, x, y);

		// get the tile at the clicked position
		return m_Grid.GetObject(x, y);
	}

	void MineField::FloodPlain(GridObject& tile)
	{
		// Check if the current tile is not already safe or a bomb.
		if (tile.GetTileType() != TileType::Safe && tile.GetTileType() != TileType::Bomb)
		{
			// Set tile to safe
			tile.SetTileType(TileType::Safe);
			m_GridRenderer.SetObjectTexture(tile.GetPosition(), "SafeTile");

			// Flood the surrounding tiles recursively.
			for (auto& neighbour : tile.SurroundingTiles())
			{
				if (neighbour)
				{
					// Is there a bomb nearby
					uint8_t amount = neighbour->FindSurroundingBombs();
					if (amount > 0 && neighbour->GetTileType() != TileType::Bomb)
					{
						neighbour->SetTileType(TileType::Safe);

						// tell the user how many bombs nearby
						std::string s = std::to_string(amount);
						m_GridRenderer.SetObjectTexture(neighbour->GetPosition(), s);

						continue;
					}

					FloodPlain(*neighbour);
				}
			}
		}
	}

	void MineField::PlayerChoice(sf::RenderWindow& window)
	{
		// click on a tile to see if its a bomb or a safe tile
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			while (sf::Mouse::isButtonPressed(sf::Mouse::Left));

			TilePicker(window);
		}
		// place flag on a tile
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			while (sf::Mouse::isButtonPressed(sf::Mouse::Right));

			FlagPlacer(window);
		}
	}

	void MineField::TilePicker(sf::RenderWindow& window)
	{
		GridObject* tile = GetTileAtPosition(window);

		if (tile)
		{
			// player clicked on a bomb tile
			if (tile->GetTileType() == TileType::Bomb)
			{
				GameOver(*tile);
			}
			else
			{
				uint8_t amount = tile->FindSurroundingBombs();
				if (amount > 0)
				{
					// set tile to safe
					tile->SetTileType(TileType::Safe);

					// tell player bomb amount
					std::string s = std::to_string(amount);
					m_GridRenderer.SetObjectTexture(tile->GetPosition(), s);
				}
				else
				{
					// tell the player the next set of bombs by doing a floodplain
					FloodPlain(*tile);
				}
			}
		}
	}

	void MineField::FlagPlacer(sf::RenderWindow& window)
	{
		GridObject* tile = GetTileAtPosition(window);

		if (tile)
		{
			// remove flag
			if (tile->GetFlagState())
			{
				tile->SetFlag(false);
				m_GridRenderer.SetObjectTexture(tile->GetPosition(), "Tile");
			}
			// place flag
			else if (tile->GetTileType() != TileType::Safe)
			{
				tile->SetFlag(true);
				m_GridRenderer.SetObjectTexture(tile->GetPosition(), "Flag");
			}
		}
	}

	void MineField::GameOver(GridObject& tile)
	{
		// show the selected bomb
		m_GridRenderer.SetObjectTexture(tile.GetPosition(), "SelectedBomb");

		// show the rest of the bombs
		for (auto& tiles : m_Grid.GetGrid())
		{
			if (tiles->GetTileType() == TileType::Bomb)
			{
				if (tiles == &tile) continue;
				m_GridRenderer.SetObjectTexture(tiles->GetPosition(), "Bomb");
			}
		}

		m_GameState = GameState::GameOver;
	}

	void MineField::DrawField(sf::RenderWindow& window)
	{
		m_GridRenderer.Draw(window);
	}

}