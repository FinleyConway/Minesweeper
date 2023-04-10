#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "Grid/GridObject.hpp"
#include "Grid/GridRenderer.hpp"

namespace Minesweeper {

	/*
	* Easy = 8x8, 10 mines
	* Medium = 16x16, 40 mines
	* Hard = 30x16, 99 mines
	*/

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
		MineField(Difficulty difficulty, TextureManager& textureManager);

		void PlayMinesweeper(Difficulty difficulty, sf::RenderWindow& window);

		void DrawField(sf::RenderWindow& window);

	private:
		Grid<GridObject> m_Grid;
		GridRenderer<GridObject> m_GridRenderer;

		enum class GameState { Start, Playing, GameOver };
		GameState m_GameState = GameState::Start;

		void WaitForFirstAction(Difficulty difficulty, sf::RenderWindow& window);

		void PlaceMines(Vector2Int initPosition, Difficulty difficulty);
		void FloodPlain(GridObject& tile);
		GridObject* GetTileAtPosition(sf::RenderWindow& window);

		void PlayerChoice(sf::RenderWindow& window);
		void TilePicker(sf::RenderWindow& window);
		void FlagPlacer(sf::RenderWindow& window);

		void GameOver(GridObject& tile);
	};
}