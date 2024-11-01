#include "TextureManager.hpp"

namespace Minesweeper {

	bool TextureManager::LoadFromFile(const std::string& name, const std::string& filePath)
	{
		Texture texture(filePath);
		bool result = texture.Exists();

		if (result)
		{
			m_Texture.emplace(name, std::move(texture));
		}

		return result;
	}

	void TextureManager::UnLoad(const std::string& name)
	{
		m_Texture.erase(name);
	}

	void TextureManager::ClearAll()
	{
		m_Texture.clear();
	}

	bool TextureManager::HasTexture(const std::string& name)
	{
		auto find = m_Texture.find(name);
		return find != m_Texture.end();
	}

	Texture& TextureManager::GetTexture(const std::string& name)
	{
		auto texture = m_Texture.find(name);
		return texture->second;
	}

}
