#pragma once

#include <memory>

#include "SfmlHandler.hpp"

namespace Minesweeper {

	class TextureManager;

	struct Texture
	{
	public:
		Texture(Texture&& move) noexcept : m_Texture(std::move(move.m_Texture)) { } // allow moving
		Texture& operator=(Texture&& move) noexcept { std::swap(m_Texture, move.m_Texture); }

		Texture(Texture const&) = delete; // prevent coping
		Texture& operator=(Texture const&) = delete;
		
		sf::Texture& GetTexture() const { return *m_Texture; }

	private:
		friend class TextureManager;

		Texture(const std::string& filePath)
		{
			sf::Texture texture;

			if (!texture.loadFromFile(filePath))
			{
				m_Texture = nullptr;
			}
			m_Texture = std::make_unique<sf::Texture>(texture);
		}

		bool Exists() const { return m_Texture.get(); }

		std::unique_ptr<sf::Texture> m_Texture;
	};

	class TextureManager
	{
	public:
		bool LoadFromFile(const std::string& name, const std::string& filePath);
		void UnLoad(const std::string& name);
		void ClearAll();

		bool HasTexture(const std::string& name);
		Texture& GetTexture(const std::string& name);

	private:
		std::map<std::string, Texture> m_Texture;
	};

}