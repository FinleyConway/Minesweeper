#pragma once

#include "SfmlHandler.hpp"
#include "TextureManager.hpp"
#include "Game.hpp"

namespace Minesweeper {

	inline void MainLoop()
	{
		SfmlHandler sfmlHandler("Minesweeper", 256, 256);

		TextureManager textureManager;
		textureManager.LoadFromFile("1", "assets/1.png");
		textureManager.LoadFromFile("2", "assets/2.png");
		textureManager.LoadFromFile("3", "assets/3.png");
		textureManager.LoadFromFile("4", "assets/4.png");
		textureManager.LoadFromFile("5", "assets/5.png");
		textureManager.LoadFromFile("6", "assets/6.png");
		textureManager.LoadFromFile("7", "assets/7.png");
		textureManager.LoadFromFile("8", "assets/8.png");
		textureManager.LoadFromFile("Bomb", "assets/Bomb.png");
		textureManager.LoadFromFile("Flag", "assets/Flag.png");
		textureManager.LoadFromFile("SafeTile", "assets/HoverTile.png");
		textureManager.LoadFromFile("SelectedBomb", "assets/SelectedBomb.png");
		textureManager.LoadFromFile("Tile", "assets/Tile.png");

		Game game(sfmlHandler, textureManager);

		while (sfmlHandler.Window().isOpen())
		{
			game.PollEvent();
			game.Update();
			game.Render();
		}
	}

}