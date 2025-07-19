#pragma once
struct Matrix;

struct Quat
{
	// order is important for streaming and RW compatibility
	float x, y, z, w;
};
struct V3d
{
	float x, y, z;
	void set(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
	}
	//static void transformPoints(V3d* out, const V3d* in, int32_t n, const Matrix* m);
	//static void transformVectors(V3d* out, const V3d* in, int32_t n, const Matrix* m);
};
struct Matrix
{
	enum Type {
		TYPENORMAL = 1,
		TYPEORTHOGONAL = 2,
		TYPEORTHONORMAL = 3,
		TYPEMASK = 3
	};
	enum Flags {
		IDENTITY = 0x20000
	};
	struct Tolerance {
		float normal;
		float orthogonal;
		float identity;
	};

	V3d right;
	uint32_t flags;
	V3d up;
	uint32_t pad1;
	V3d at;
	uint32_t pad2;
	V3d pos;
	uint32_t pad3;

	
};

//typedef RV3d V3d;
class CVector : public V3d
{
public:
	CVector(void) {}
	CVector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	CVector(const V3d &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	// (0,1,0) means no rotation. So get right vector and its atan
	

	const CVector &operator+=(CVector const &right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	const CVector &operator-=(CVector const &right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	const CVector &operator*=(float right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	const CVector &operator/=(float right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	CVector operator-() const {
		return CVector(-x, -y, -z);
	}

	const bool operator==(CVector const &right) const {
		return x == right.x && y == right.y && z == right.z;
	}

	const bool operator!=(CVector const &right) const {
		return x != right.x || y != right.y || z != right.z;
	}

	bool IsZero(void) const { return x == 0.0f && y == 0.0f && z == 0.0f; }
};

inline CVector operator+(const CVector &left, const CVector &right)
{
	return CVector(left.x + right.x, left.y + right.y, left.z + right.z);
}

inline CVector operator-(const CVector &left, const CVector &right)
{
	return CVector(left.x - right.x, left.y - right.y, left.z - right.z);
}

inline CVector operator*(const CVector &left, float right)
{
	return CVector(left.x * right, left.y * right, left.z * right);
}

inline CVector operator*(float left, const CVector &right)
{
	return CVector(left * right.x, left * right.y, left * right.z);
}

inline CVector operator/(const CVector &left, float right)
{
	return CVector(left.x / right, left.y / right, left.z / right);
}

inline float
DotProduct(const CVector &v1, const CVector &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

CVector CrossProduct(const CVector &v1, const CVector &v2);





//class CMatrix;

//CVector Multiply3x3(const CMatrix &mat, const CVector &vec);
//CVector Multiply3x3(const CVector &vec, const CMatrix &mat);
//CVector operator*(const CMatrix &mat, const CVector &vec);