#pragma once

#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <format>
#include <random>

namespace Minesweeper {

	template<typename TTile>
	class Grid
	{
	public:
		Grid() = default;

		uint32_t GetWidth() const { return m_SizeX; }
		uint32_t GetHeight() const { return m_SizeY; }
		uint16_t GetCellSize() const { return m_CellSize; }
		const std::vector<TTile>& GetTiles() const { return m_Tiles; }

		/**
		 * Create grid with expected size.
		 * @param sizeX Grid size x.
		 * @param sizeY Grid Size y.
		 * @param cellSize Grid tile size.
		 */
		void Create(uint32_t sizeX, uint32_t sizeY, uint16_t cellSize)
		{
			m_Tiles.resize(sizeX * sizeY);
			m_SizeX = sizeX;
			m_SizeY = sizeY;
			m_CellSize = cellSize;
		}

		/**
		 * Get tile at grid position.
		 * @param x Grid position x.
		 * @param y Grid position y.
		 * @return The tile.
		 */
		TTile& Get(int32_t x, int32_t y)
		{
			if (!IsWithin(x, y)) throw std::out_of_range(std::format("Grid<>: Out of grid bounds at x:{}, y:{}!", x, y));

			return m_Tiles[x + y * m_SizeX];
		}

		/**
		 * Set tile at grid position.
		 * @param x Grid position x.
		 * @param y Grid position y.
		 * @param tile The tile.
		 */
		void Set(int32_t x, int32_t y, TTile& tile)
		{
			if (IsWithin(x, y))
			{
				m_Tiles[x + y * m_SizeX] = tile;
			}
		}

		/**
		 * Set tile at index.
		 * @param index Index to tile.
		 * @param tile The tile.
		 */
		void Set(int32_t index, TTile& tile)
		{
			if (index >= 0 && index < m_Tiles.size())
			{
				m_Tiles[index] = tile;
			}
		}

		std::vector<std::pair<int32_t, int32_t>> GetSurrounding(int32_t x, int32_t y) const
		{
			static const std::array directions =
			{
				sf::Vector2i(+0, -1),
				sf::Vector2i(+0, +1),
				sf::Vector2i(-1, +0),
				sf::Vector2i(+1, +0),
				sf::Vector2i(-1, -1),
				sf::Vector2i(+1, -1),
				sf::Vector2i(-1, +1),
				sf::Vector2i(+1, +1)
			};

			std::vector<std::pair<int32_t, int32_t>> tiles;
			tiles.reserve(8); // reserve all possible tiles

			for (const auto direction : directions)
			{
				const int32_t posX = direction.x + x;
				const int32_t posY = direction.y + y;

				if (IsWithin(posX, posY))
				{
					tiles.emplace_back(posX, posY);
				}
			}

			return tiles;
		}

		/**
		 * Checks if x and y are within the grid.
		 * @param x Grid position x.
		 * @param y Grid position y.
		 * @return Is within or not.
		 */
		bool IsWithin(int32_t x, int32_t y) const
		{
			return x >= 0 && x < m_SizeX && y >= 0 && y < m_SizeY;
		}

		/**
		 * Reset grid to default.
		 */
		void Clear()
		{
			m_Tiles.clear();
			m_SizeX = 0;
			m_SizeY = 0;
			m_CellSize = 0;
		}

		/**
		 * Convert grid position to screen position.
		 * @param x Grid position x.
		 * @param y Grid position y.
		 * @return Screen position.
		 */
		std::pair<float, float> ConvertGridToScreen(int32_t x, int32_t y) const
		{
			return { x * m_CellSize, y * m_CellSize };
		}

		/**
		 * Convert screen position to grid position.
		 * @param screenX Screen position x.
		 * @param screenY Screen position y.
		 * @return Grid position.
		 */
		std::pair<int32_t, int32_t> ConvertScreenToGrid(float screenX, float screenY) const
		{
			// avoid dividing by 0
			if (m_CellSize == 0)
			{
				std::cout << "Trying to divide with 0 when convert to grid position";
				return { 0, 0 };
			}

			float gridX = std::floor(screenX / m_CellSize);
			float gridY = std::floor(screenY / m_CellSize);

			return { gridX, gridY };
		}

	private:
		std::vector<TTile> m_Tiles;
		uint32_t m_SizeX = 0;
		uint32_t m_SizeY = 0;
		uint16_t m_CellSize = 0;
	};

} // Minesweeper