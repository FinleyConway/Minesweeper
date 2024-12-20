#pragma once

#include <cstdint>

namespace Minesweeper {

	enum class TileType { Ground, Safe, Mine, SelectedMine };

	struct Tile
	{
		uint8_t numberOfMines = 0;
		TileType type = TileType::Ground;
		bool flagged = false;
	};

} // Minesweeper