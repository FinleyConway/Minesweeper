#include "Minesweeper.h"

namespace Minesweeper {

	State Minesweeper::CreateField(uint32_t sizeX, uint32_t sizeY, uint16_t cellSize)
	{
		// clear pre-existing field
		m_Grid.Clear();

		// create the field
		m_Grid.Create(sizeX, sizeY, cellSize);

		return State::Started;
	}

	State Minesweeper::SelectInitialTile(uint32_t numOfMines, float mouseX, float mouseY)
	{
		const auto [x, y] = m_Grid.ConvertScreenToGrid(mouseX, mouseY);

		if (!m_Grid.IsWithin(x, y)) return State::Started;

		auto& tile = m_Grid.Get(x, y);

		if (tile.flagged) return State::Started;

		// place mines
		PlaceMines(numOfMines, x, y);

		// create working area
		FloodPlain(tile, x, y);

		return State::Playing;
	}

	State Minesweeper::SelectTile(float mouseX, float mouseY)
	{
		const auto [x, y] = m_Grid.ConvertScreenToGrid(mouseX, mouseY);

		if (!m_Grid.IsWithin(x, y)) return State::Playing;

		auto& tile = m_Grid.Get(x, y);

		if (tile.flagged) return State::Playing;

		if (tile.type == TileType::Mine)
		{
			tile.type = TileType::SelectedMine;
			return State::GameOver;
		}

		const uint8_t numOfMines = SurroundingMineCount(x, y);

		// set tile state
		if (numOfMines > 0)
		{
			tile.type = TileType::Safe;
			tile.numberOfMines = numOfMines;
		}
		else FloodPlain(tile, x, y);

		return State::Playing;
	}

	void Minesweeper::SetFlag(float mouseX, float mouseY)
	{
		const auto [x, y] = m_Grid.ConvertScreenToGrid(mouseX, mouseY);

		if (!m_Grid.IsWithin(x, y)) return;

		auto& tile = m_Grid.Get(x, y);

		if (tile.type == TileType::Ground || tile.type == TileType::Mine)
		{
			tile.flagged = !tile.flagged;
		}
	}

	const std::vector<Tile>& Minesweeper::GetTiles() const { return m_Grid.GetTiles(); }

	uint8_t Minesweeper::SurroundingMineCount(int32_t x, int32_t y)
	{
		uint8_t numOfMines = 0;

		// get the number of surrounding mines
		for (auto [x, y] : m_Grid.GetSurrounding(x, y))
		{
			const auto& surroundingTile = m_Grid.Get(x, y);

			if (surroundingTile.type == TileType::Mine) numOfMines++;
		}

		return numOfMines;
	}

	void Minesweeper::PlaceMines(uint32_t numOfMines, int32_t excludeX, int32_t excludeY)
	{
		if (!m_Grid.IsWithin(excludeX, excludeY)) return;

		const uint32_t width = m_Grid.GetWidth();
		const uint32_t height = m_Grid.GetHeight();

		std::vector<std::pair<uint32_t, uint32_t>> availableCells;
		availableCells.reserve(width * height);

		// get all non excluded tiles
		for (int32_t x = 0; x < width; x++)
		{
			for (int32_t y = 0; y < height; y++)
			{
				// skip all cells around an area
				constexpr uint32_t area = 1;
				if (std::abs(x - excludeX) <= area && std::abs(y - excludeY) <= area) continue;

				availableCells.emplace_back(x, y);
			}
		}

		// shuffles the tiles
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(availableCells.begin(), availableCells.end(), gen);

		// place bombs in the random list of positions
		for (uint32_t i = 0; i < numOfMines; ++i)
		{
			auto  [x, y] = availableCells[i];
			auto& tile = m_Grid.Get(x, y);

			tile.type = TileType::Mine;
		}
	}

	void Minesweeper::FloodPlain(Tile& tile, int32_t tileX, int32_t tileY)
	{
		if (!m_Grid.IsWithin(tileX, tileY)) return;

		// mark tile as safe
		tile.type = TileType::Safe;
		tile.flagged = false;
		tile.numberOfMines = SurroundingMineCount(tileX, tileY);

		// stop flood if there is a surrounding mine
		if (tile.numberOfMines > 0) return;

		// continue flood if there are no surrounding mines
		for (auto [x, y] : m_Grid.GetSurrounding(tileX, tileY))
		{
			auto& surroundingTile = m_Grid.Get(x, y);

			// only flood cells that have not been visited yet
			if (surroundingTile.type != TileType::Safe) FloodPlain(surroundingTile, x, y);
		}
	}

} // Minesweeper