#include "MineField.hpp"

namespace Minesweeper {

	MineField::MineField(Difficulty difficulty, TextureManager& textureManager) :
		m_Grid(Grid<GridObject>(difficulty.GridSize.X(), difficulty.GridSize.Y(), 32, Vector2::Zero(), [](int32_t x, int32_t y, Grid<GridObject>& g) { return GridObject(x, y, g); })),
		m_GridRenderer(m_Grid, textureManager)
	{
		for (auto& tile : m_Grid.GetGrid())
		{
			tile->SetTexture(textureManager.GetTexture("Tile"));
		}
	}

	void MineField::PlayMinesweeper(Difficulty difficulty, sf::RenderWindow& window)
	{
		if (m_GameState == GameState::Start)
		{
			WaitForFirstAction(difficulty, window);
		}
		else if (m_GameState == GameState::Playing)
		{
			PlayerChoice(window);
		}
		else if (m_GameState == GameState::GameOver)
		{

		}
	}

	void MineField::WaitForFirstAction(Difficulty difficulty, sf::RenderWindow& window)
	{
		// check if the left mouse button is pressed and the game has not yet started
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			GridObject* tile = GetTileAtPosition(window);

			if (tile)
			{
				// place mines on the game grid, except for the clicked tile
				PlaceMines(tile->GetPosition(), difficulty);

				// initialize floodplain
				FloodPlain(*tile);

				m_GameState = GameState::Playing;
			}
		}
	}

	void MineField::PlaceMines(Vector2Int initPosition, Difficulty difficulty)
	{
		// initialize random number generator
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> randomNumber(0, m_Grid.GetWidth() - 1);

		std::vector<Vector2Int> takenPositions;
		int32_t bombsPlaced = 0;
		int32_t totalTiles = m_Grid.GetWidth() * m_Grid.GetHeight();

		// keep placing bombs until the desired number is reached or there are no more available positions
		while (bombsPlaced < difficulty.MinesAmount && takenPositions.size() < totalTiles)
		{
			// generate a random position on the game grid
			Vector2Int position(randomNumber(rng), randomNumber(rng));

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

		// get the tile at the clicked position
		int32_t x, y;
		m_Grid.ConvertScreenToGrid(position, x, y);
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