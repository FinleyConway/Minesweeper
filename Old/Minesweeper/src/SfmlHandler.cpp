#include "SfmlHandler.hpp"

namespace Minesweeper {

	SfmlHandler::SfmlHandler(const std::string& title, uint32_t width, uint32_t height) :
		m_Window(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar)
	{
	}
}