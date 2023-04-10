#pragma once

#include "SfmlHandler.hpp"
#include "TextureManager.hpp"
#include "MineField.hpp"

namespace Minesweeper {

	class Game
	{
	public:
		Game(SfmlHandler& sfmlHandler, TextureManager& textureManager);

		void PollEvent();
		void Update();
		void Render();

	private:
		SfmlHandler& m_Handler;
		TextureManager& m_TextureManager;

		MineField m_MineField;
	};

}