#pragma once

#include <math.h>
#include <string>

#include "Vector2Int.hpp"

namespace Minesweeper {

    struct Vector2
    {
    public:
        Vector2() = default;
        Vector2(float x, float y) : m_X(x), m_Y(y) { }

        // static properties
        static Vector2 Up() { return Vector2(0, 1); }
        static Vector2 Down() { return Vector2(0, -1); }
        static Vector2 Right() { return Vector2(1, 0); }
        static Vector2 Left() { return Vector2(-1, 0); }
        static Vector2 Zero() { return Vector2(0, 0); }

        // properties
        // get
        float X() const { return m_X; }
        float Y() const { return m_Y; }

        // set
        void X(float x) { m_X = x; }
        void Y(float y) { m_Y = y; }

        float Magnitude() const { return sqrtf(m_X * m_X + m_Y * m_X); }

        void Normalize() 
        {
            float l = Magnitude();
            if (l > 0) {
                (*this) *= 1 / l;
            }
        }

        // methods
        void Clamp(Vector2& min, Vector2& max)
        {
            if (m_X < min.m_X) m_X = min.m_X;
            if (m_Y > max.m_Y) m_Y = max.m_Y;
        }

        std::string ToString()
        {
            return std::to_string(m_X) + " " + std::to_string(m_Y);
        }

        // static methods
        static float Distance(Vector2& v1, Vector2& v2)
        {
            return (v1 - v2).Magnitude();
        }

        // operators

        // adding
        Vector2 operator+(const Vector2& v1) const
        {
            return Vector2(m_X + v1.m_X, m_Y + v1.m_Y);
        }
        friend Vector2& operator+=(Vector2& v1, const Vector2& v2)
        {
            v1.m_X += v2.m_X;
            v1.m_Y += v2.m_Y;
            return v1;
        }

        // multiplying
        Vector2 operator*(float scalar)
        {
            return Vector2(m_X * scalar, m_Y * scalar);
        }
        Vector2& operator*=(float scalar) {
            m_X *= scalar;
            m_Y *= scalar;
            return *this;
        }

        // subtracting
        Vector2 operator-(const Vector2& v1) const
        {
            return Vector2(m_X - v1.m_X, m_Y - v1.m_Y);
        }
        friend Vector2& operator-=(Vector2& v1, const Vector2& v2) {
            v1.m_X -= v2.m_X;
            v1.m_Y -= v2.m_Y;
            return v1;
        }

        // dividing
        Vector2 operator/(float scalar)
        {
            return Vector2(m_X / scalar, m_Y / scalar);
        }
        Vector2& operator/=(float scalar) {
            m_X /= scalar;
            m_Y /= scalar;
            return *this;
        }

        // equal to
        bool operator==(const Vector2& v1) const
        {
            return m_X == v1.m_X && m_Y == v1.m_Y;
        }

        // does not equal to
        bool operator!=(const Vector2& v1) const
        {
            return m_X != v1.m_X && m_Y != v1.m_Y;
        }

    private:
        float m_X;
        float m_Y;
    };

}