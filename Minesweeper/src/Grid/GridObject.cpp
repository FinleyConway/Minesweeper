#include "GridObject.hpp"

namespace Minesweeper {

    const Vector2Int GridObject::s_AdjacentOffsets[8] =
    {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    GridObject::GridObject(int32_t x, int32_t y, Grid<GridObject>& grid) :
        m_Grid(grid),
        m_Position(x, y)
    {
    }

    void GridObject::SetTexture(Texture& texture) 
    { 
        m_Sprite.setTexture(texture.GetTexture()); 
        m_Sprite.setPosition(m_Position.X() * m_Grid.GetCellSize(), m_Position.Y() * m_Grid.GetCellSize());
    }

    uint8_t GridObject::FindSurroundingBombs()
    {
        uint8_t amount = 0;
        for (auto& tile : SurroundingTiles())
        {
            if (tile && tile->m_TileType == TileType::Bomb)
            {
                amount++;
            }
        }
        return amount;
    }

    std::vector<GridObject*> GridObject::SurroundingTiles()
    {
        std::vector<GridObject*> surrounding;

        for (auto& position : s_AdjacentOffsets)
        {
            int x = position.X() + m_Position.X();
            int y = position.Y() + m_Position.Y();
            surrounding.push_back(m_Grid.GetObject(x, y));
        }

        return surrounding;
    }

}