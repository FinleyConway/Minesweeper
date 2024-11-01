#pragma once

#include <functional>
#include <vector>
#include <math.h>

#include "../Vector2.hpp"

namespace Minesweeper {

    template<class T>
    class Grid
    {
    public:
        Grid(int32_t width, int32_t height, uint32_t cellSize, Vector2 originPosition, std::function<T(int, int, Grid<T>&)> gridObject) 
        {
            m_Width = width;
            m_Height = height;
            m_CellSize = cellSize;
            m_OriginPosition = originPosition;
            m_Grid.resize(width * height);

            for (int32_t x = 0; x < width; x++)
            {
                for (int32_t y = 0; y < height; y++)
                {
                    m_Grid[x + y * width] = new T(gridObject(x, y, *this));
                }
            }
        }

        ~Grid()
        {
            for (auto& element : m_Grid)
            {
                delete element;
            }

            m_Grid.clear();
        }

        int32_t GetWidth() const { return m_Width; }
        int32_t GetHeight() const { return m_Height; }
        uint32_t GetCellSize() const { return m_CellSize; }
        const std::vector<T*>& GetGrid() const { return m_Grid; }

        // set object from grid using x and y
        void SetObject(int32_t x, int32_t y, T& object)
        {
            if (x >= 0 && y >= 0 && x < m_Width && y < m_Height)
            {
                m_Grid[x + y * m_Width] = object;
            }
        }

        // set object from grid using x and y
        void SetObject(Vector2Int position, T& object)
        {
            if (position.X() >= 0 && position.Y() >= 0 && position.X() < m_Width && position.Y() < m_Height)
            {
                m_Grid[position.X() + position.Y() * m_Width] = object;
            }
        }

        // set object from grid using vector position
        void SetObject(Vector2 position, T& object)
        {
            int x, y;
            ConvertScreenToGrid(position, x, y);
            SetObject(x, y, object);
        }

        // get object from grid using x and y
        T* GetObject(int32_t x, int32_t y)
        {
            if (x >= 0 && y >= 0 && x < m_Width && y < m_Height)
            {
                return m_Grid[x + y * m_Width];
            }
            return nullptr;
        }

        // get object from grid using vector int position
        T* GetObject(Vector2Int position)
        {
            if (position.X() >= 0 && position.Y() >= 0 && position.X() < m_Width && position.Y() < m_Height)
            {
                return m_Grid[position.X() + position.Y() * m_Width];
            }
            return nullptr;
        }

        // get object from grid using vector position
        T* GetObject(Vector2 position)
        {
            int32_t x, y;
            ConvertScreenToGrid(position, x, y);
            return GetObject(x, y);
        }

        // convert grid position to screen position
        Vector2 ConvertGridToScreen(int32_t x, int32_t y)
        {
            return Vector2(x, y) * m_CellSize + m_OriginPosition;
        }

        // converts mouse position to grid position
        void ConvertScreenToGrid(Vector2 mousePosition, int32_t& x, int32_t& y)
        {
            x = floorf((mousePosition - m_OriginPosition).X() / m_CellSize);
            y = floorf((mousePosition - m_OriginPosition).Y() / m_CellSize);
        }

        // check if the x and y is in the matrix
        bool IsInsideGrid(int32_t x, int32_t y)
        {
            return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
        }

    private:
        int32_t m_Width = 0;
        int32_t m_Height = 0;
        uint32_t m_CellSize = 0;
        Vector2 m_OriginPosition = Vector2::Zero();
        std::vector<T*> m_Grid;
    };

}