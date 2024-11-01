#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Grid.hpp"
#include "../Vector2Int.hpp"
#include "../TextureManager.hpp"

namespace Minesweeper {

    enum class TileType
    {
        Normal, Safe, Bomb
    };

    class GridObject
    {
    public:
        GridObject(int32_t x, int32_t y, Grid<GridObject>& grid);

        void SetTileType(TileType tileType) { m_TileType = tileType; }
        void SetTexture(Texture& texture);
        void SetFlag(bool isFlagged) { m_IsFlagged = isFlagged; }

        const sf::Sprite& GetSprite() const { return m_Sprite; }
        Vector2Int GetPosition() const { return m_Position; }
        TileType GetTileType() const { return m_TileType; }
        bool GetFlagState() const { return m_IsFlagged; }

        std::vector<GridObject*> SurroundingTiles();
        uint8_t FindSurroundingBombs();

    private:
        sf::Sprite m_Sprite;
        Vector2Int m_Position = Vector2Int::Zero();
        Grid<GridObject>& m_Grid;

        TileType m_TileType = TileType::Normal;
        bool m_IsFlagged = false;

        static const Vector2Int s_AdjacentOffsets[8];
    };
}