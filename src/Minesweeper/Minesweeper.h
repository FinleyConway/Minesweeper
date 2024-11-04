#pragma once

#include "Grid.h"
#include "Tile.h"

namespace Minesweeper {

	/**
	 * Game state.
	 */
	enum class State { Started, Playing, GameOver };

	class Minesweeper
	{
	public:
		/**
		 * Create the game with given field dimensions.
		 * @param sizeX The width of the field.
		 * @param sizeY The height of the field.
		 * @param cellSize The size of each tile.
		 */
		State CreateField(uint32_t sizeX, uint32_t sizeY, uint16_t cellSize);

		/**
		 * The initial function to be called to start the game.
		 * @param numOfMines The amount of mines to be placed.
		 * @param mouseX The mouse position x.
		 * @param mouseY The mouse position y.
		 * @return The game state after calling this.
		 */
		State SelectInitialTile(uint32_t numOfMines, float mouseX, float mouseY);

		/**
		 * Select a tile from the grid.
		 * @param mouseX The mouse position x.
		 * @param mouseY The mouse position y.
		 * @return The game state after calling this.
		 */
		State SelectTile(float mouseX, float mouseY);

		/**
		 * Allow to set a flag on a tile.
		 * @param mouseX The mouse position x.
		 * @param mouseY The mouse position y.
		 */
		void SetFlag(float mouseX, float mouseY);

		/**
		 * An immutable reference to all the tiles in the field.
		 * @return A list of tiles.
		 */
		const std::vector<Tile>& GetTiles() const;

	private:
		/**
		 * Get the number of mines around a tile.
		 * @param x The grid position x.
		 * @param y The grid position y.
		 * @return The number of mines.
		 */
		uint8_t SurroundingMineCount(int32_t x, int32_t y);

		/**
		 * Places mines randomly around the field.
		 * @param numOfMines The amount of mines to be placed.
		 * @param excludeX Where mines shouldn't be placed on the x.
		 * @param excludeY Where mines shouldn't be placed on the y.
		 */
		void PlaceMines(uint32_t numOfMines, int32_t excludeX, int32_t excludeY);

		/**
		 * Recursively reveals the field
		 * @param tile Starting tile.
		 * @param tileX The x position of the starting tile.
		 * @param tileY The y position of the starting tile.
		 */
		void FloodPlain(Tile& tile, int32_t tileX, int32_t tileY);

	private:
		Grid<Tile> m_Grid;
	};

} // Minesweeper