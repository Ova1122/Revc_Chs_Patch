#pragma once

class CVector2D
{
public:
	float x, y;
	CVector2D(void) {}
	CVector2D(float x, float y) : x(x), y(y) {}
	CVector2D(const CVector &v) : x(v.x), y(v.y) {}
	



	const CVector2D &operator+=(CVector2D const &right) {
		x += right.x;
		y += right.y;
		return *this;
	}

	const CVector2D &operator-=(CVector2D const &right) {
		x -= right.x;
		y -= right.y;
		return *this;
	}

	const CVector2D &operator*=(float right) {
		x *= right;
		y *= right;
		return *this;
	}

	const CVector2D &operator/=(float right) {
		x /= right;
		y /= right;
		return *this;
	}
	CVector2D operator-(const CVector2D &rhs) const {
		return CVector2D(x-rhs.x, y-rhs.y);
	}
	CVector2D operator+(const CVector2D &rhs) const {
		return CVector2D(x+rhs.x, y+rhs.y);
	}
	CVector2D operator/(float t) const {
		return CVector2D(x/t, y/t);
	}
	CVector2D operator-() const {
		return CVector2D(-x, -y);
	}
};



inline float
DistanceSqr2D(const CVector2D &v, float x, float y)
{
	return (v.x-x)*(v.x-x) + (v.y-y)*(v.y-y);
}


inline CVector2D operator*(const CVector2D &left, float right)
{
	return CVector2D(left.x * right, left.y * right);
}

inline CVector2D operator*(float left, const CVector2D &right)
{
	return CVector2D(left * right.x, left * right.y);
}
