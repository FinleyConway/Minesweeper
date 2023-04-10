#pragma once

#include "Grid.hpp"
#include "../TextureManager.hpp"

namespace Minesweeper {

	template<class T>
	class GridRenderer
	{
	public:
		GridRenderer(Grid<T>& grid, TextureManager& textureManager) :
			m_Grid(grid),
			m_TextuerManager(textureManager)
		{
		}

		void Draw(sf::RenderWindow& window)
		{
			for (auto& tile : m_Grid.GetGrid())
			{
				window.draw(tile->GetSprite());
			}
		}

		// set object texture directly
		void SetObjectTexture(GridObject& gridObject, const std::string& name)
		{
			std::vector<T*> grid = m_Grid.GetGrid();
			auto it = std::find_if(grid.begin(), grid.end(), gridObject);

			if (it != grid.end()) 
			{
				it->SetTexture(GetTexture(name));
			}
		}

		// set object texture from grid using x and y
		void SetObjectTexture(int32_t x, int32_t y, const std::string& name)
		{
			if (x >= 0 && y >= 0 && x < m_Grid.GetWidth() && y < m_Grid.GetWidth())
			{
				m_Grid.GetGrid()[x + y * m_Grid.GetWidth()]->SetTexture(GetTexture(name));
			}
		}

		// set object text from grid using vector int position
		void SetObjectTexture(Vector2Int position, const std::string& name)
		{
			if (position.X() >= 0 && position.Y() >= 0 && position.X() < m_Grid.GetWidth() && position.Y() < m_Grid.GetWidth())
			{
				m_Grid.GetGrid()[position.X() + position.Y() * m_Grid.GetWidth()]->SetTexture(GetTexture(name));
			}
		}

		// set object texture from grid using vector position
		void SetObjectTexture(Vector2 position, const std::string& name)
		{
			int x, y;
			m_Grid.ConvertScreenToGrid(position, x, y);
			SetObjectTexture(x, y, name);
		}

	private:
		Texture& GetTexture(const std::string& name)
		{
			if (m_TextuerManager.HasTexture(name))
			{
				return m_TextuerManager.GetTexture(name);
			}
		}

		Grid<T>& m_Grid;
		TextureManager& m_TextuerManager;
	};

}