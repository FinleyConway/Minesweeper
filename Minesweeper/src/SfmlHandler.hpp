#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

namespace Minesweeper {

	class SfmlHandler
	{
	public:
		SfmlHandler(const std::string& title, uint32_t width, uint32_t height);

		sf::RenderWindow& Window() { return m_Window; }

	private:
		sf::RenderWindow m_Window;
	};

}