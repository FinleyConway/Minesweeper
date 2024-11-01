#pragma once

#include "SfmlHandler.hpp"
#include "TextureManager.hpp"
#include "Game.hpp"

namespace Minesweeper {

	inline void MainLoop()
	{
		SfmlHandler sfmlHandler("Minesweeper",	30*32, 16*32);

		TextureManager textureManager;
		textureManager.LoadFromFile("1",			"assets/1.png");
		textureManager.LoadFromFile("2",			"assets/2.png");
		textureManager.LoadFromFile("3",			"assets/3.png");
		textureManager.LoadFromFile("4",			"assets/4.png");
		textureManager.LoadFromFile("5",			"assets/5.png");
		textureManager.LoadFromFile("6",			"assets/6.png");
		textureManager.LoadFromFile("7",			"assets/7.png");
		textureManager.LoadFromFile("8",			"assets/8.png");
		textureManager.LoadFromFile("Bomb",			"assets/Bomb.png");
		textureManager.LoadFromFile("Flag",			"assets/Flag.png");
		textureManager.LoadFromFile("SafeTile",		"assets/SafeTile.png");
		textureManager.LoadFromFile("SelectedBomb", "assets/SelectedBomb.png");
		textureManager.LoadFromFile("Tile",			"assets/Tile.png");

		Game game(sfmlHandler, textureManager);

		sf::Clock clock;
		const uint32_t fps = 60;
		const float loopTime = 1.0f / fps;
		float accumulator = 0.0f;

		while (sfmlHandler.Window().isOpen())
		{
			const float elasped = clock.restart().asSeconds();
			accumulator += elasped;

			while (accumulator >= loopTime)
			{
				game.PollEvent();
				game.Update();

				accumulator -= loopTime;
			}

			game.Render();
		}
	}

}