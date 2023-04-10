#include "Game.hpp"

namespace Minesweeper {

	Game::Game(SfmlHandler& sfmlHandler, TextureManager& textureManager) :
		m_Handler(sfmlHandler),
		m_TextureManager(textureManager),
		m_MineField(Difficulty(Vector2Int(8, 8), 10), textureManager)
	{
	}

	void Game::PollEvent()
	{
		sf::Event event;
		
		while (m_Handler.Window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_Handler.Window().close();
		}
	}

	void Game::Update()
	{
		m_MineField.PlayMinesweeper(Difficulty(Vector2Int(8, 8), 10), m_Handler.Window());
	}

	void Game::Render()
	{
		m_Handler.Window().clear();

		m_MineField.DrawField(m_Handler.Window());

		m_Handler.Window().display();
	}

}