#include "Game.hpp"

namespace Minesweeper {

	Game::Game(SfmlHandler& sfmlHandler, TextureManager& textureManager) :
		m_Handler(sfmlHandler),
		m_TextureManager(textureManager),
		m_MineField(sfmlHandler.Window(), textureManager)
	{
	}

	void Game::PollEvent()
	{
		sf::Event event;
		
		while (m_Handler.Window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_Handler.Window().close();
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect view(0, 0, event.size.width, event.size.height);
				m_Handler.Window().setView(sf::View(view));
			}
		}
	}

	void Game::Update()
	{
		m_MineField.PlayMinesweeper(m_Handler.Window());
	}

	void Game::Render()
	{
		m_Handler.Window().clear();

		m_MineField.DrawField(m_Handler.Window());

		m_Handler.Window().display();
	}

}