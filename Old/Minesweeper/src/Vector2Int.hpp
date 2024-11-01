#pragma once

#include <string>
#include <math.h>

namespace Minesweeper {

	struct Vector2Int
	{
	public:
		Vector2Int() = default;
		Vector2Int(int32_t x, int32_t y) : m_xPos(x), m_yPos(y) { }

		// static properties
		static Vector2Int Up() { return Vector2Int(0, 1); }
		static Vector2Int Down() { return Vector2Int(0, -1); }
		static Vector2Int Right() { return Vector2Int(1, 0); }
		static Vector2Int Left() { return Vector2Int(-1, 0); }
		static Vector2Int Zero() { return Vector2Int(0, 0); }

		// properties
		// get
		int32_t X() const { return m_xPos; }
		int32_t Y() const { return m_yPos; }

		// set
		void X(int32_t x) { m_xPos = x; }
		void Y(int32_t y) { m_yPos = y; }

		float Magnitude() const { return sqrt(m_xPos * m_xPos + m_yPos * m_xPos); }

		// methods
		void Clamp(Vector2Int& min, Vector2Int& max)
		{
			if (m_xPos < min.m_xPos) m_xPos = min.m_xPos;
			if (m_yPos > max.m_yPos) m_yPos = max.m_yPos;
		}

		std::string ToString()
		{
			return std::to_string(m_xPos) + " " + std::to_string(m_yPos);
		}

		// static methods
		static uint32_t Distance(Vector2Int& v1, Vector2Int& v2)
		{
			return (v1 - v2).Magnitude();
		}

		// operators
		// adding
		Vector2Int operator+(const Vector2Int& v1) const
		{
			return Vector2Int(m_xPos + v1.m_xPos, m_yPos + v1.m_yPos);
		}
		friend Vector2Int& operator+=(Vector2Int& v1, const Vector2Int& v2)
		{
			v1.m_xPos += v2.m_xPos;
			v1.m_yPos += v2.m_yPos;
			return v1;
		}

		// multiplying
		Vector2Int operator*(int32_t scalar)
		{
			return Vector2Int(m_xPos * scalar, m_yPos * scalar);
		}
		Vector2Int& operator*=(int32_t scalar) {
			m_xPos *= scalar;
			m_yPos *= scalar;
			return *this;
		}

		// subtracting
		Vector2Int operator-(const Vector2Int& v1) const
		{
			return Vector2Int(m_xPos - v1.m_xPos, m_yPos - v1.m_yPos);
		}
		friend Vector2Int& operator-=(Vector2Int& v1, const Vector2Int& v2) {
			v1.m_xPos -= v2.m_xPos;
			v1.m_yPos -= v2.m_yPos;
			return v1;
		}

		// dividing
		Vector2Int operator/(int32_t scalar)
		{
			return Vector2Int(m_xPos / scalar, m_yPos / scalar);
		}
		Vector2Int& operator/=(int32_t scalar) {
			m_xPos /= scalar;
			m_yPos /= scalar;
			return *this;
		}

		// equal to
		bool operator==(const Vector2Int& v1) const
		{
			return m_xPos == v1.m_xPos && m_yPos == v1.m_yPos;
		}

		// does not equal to
		bool operator!=(const Vector2Int& v1) const
		{
			return m_xPos != v1.m_xPos && m_yPos != v1.m_yPos;
		}

	private:
		int32_t m_xPos;
		int32_t m_yPos;
	};

}