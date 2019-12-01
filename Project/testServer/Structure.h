#pragma once

struct value {
	value();
	value(int);
	value(float tx, float ty, float tz);
	value operator*(float scalar);
	value operator+(const value& other);
	value operator/(float scalar);
	bool operator<(float scalar);
	float x, y, z;
};
struct box {
	float left, right, top, bottom;
	box(value pos, value volume);
	box(float l, float r, float t, float b);
	box() = default;
};
struct color {
	color();
	color(float r1, float g1, float b1, float a1);
	float r, g, b, a;
};

class Vector3D {
public:
	Vector3D() = default;
	Vector3D(float x, float y, float z);
	Vector3D(value tv);
	Vector3D& normalize();
	float size();

	Vector3D operator-(const Vector3D& rhs);
	Vector3D operator-();
	Vector3D& operator=(const Vector3D& rhs);
	Vector3D operator+(const Vector3D& rhs);
	Vector3D& operator+=(const Vector3D& rhs);
	Vector3D operator*(float _scalar);
	Vector3D operator*(const Vector3D& rhs);
	Vector3D operator/(float _scalar);

	float getX() const;
	float getY() const;
	float getZ() const;

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	void setValue(value val);
	value getValue() const;
	float distance(const Vector3D& rhs);
private:
	float length();
	value v;
};

value ptom(value& v);

bool AABBCollision(const box& lhs, const box& rhs);
float radToDegree(float rad);
float degreeToRad(float degree);


template <typename T>
constexpr const T clamp(const T& low, const T& cent, const T& high) {
	return (cent < low) ? low : (cent > high) ? high : cent;
}