#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "Grid/GridObject.hpp"
#include "Grid/GridRenderer.hpp"

namespace Minesweeper {

	struct Difficulty
	{
	public:
		Difficulty(Vector2Int size, uint32_t mineAmount)
			: 
		GridSize(size), MinesAmount(mineAmount) {}

		Vector2Int GridSize;
		uint32_t MinesAmount;
	};

	class MineField
	{
	public:
		MineField(sf::RenderWindow& window, TextureManager& textureManager);

		void PlayMinesweeper(sf::RenderWindow& window);

		void DrawField(sf::RenderWindow& window);

	private:
		Grid<GridObject> m_Grid;
		GridRenderer<GridObject> m_GridRenderer;

		enum class GameState { Start, Playing, GameOver };
		GameState m_GameState = GameState::Start;

		void WaitForFirstAction(sf::RenderWindow& window);

		void PlaceMines(Vector2Int initPosition);
		void FloodPlain(GridObject& tile);
		GridObject* GetTileAtPosition(sf::RenderWindow& window);

		void PlayerChoice(sf::RenderWindow& window);
		void TilePicker(sf::RenderWindow& window);
		void FlagPlacer(sf::RenderWindow& window);

		void GameOver(GridObject& tile);
	};
}