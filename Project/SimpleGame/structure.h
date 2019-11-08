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

	Vector3D operator-(const Vector3D& rhs);
	Vector3D& operator=(const Vector3D& rhs);
	Vector3D operator+(const Vector3D& rhs);
	Vector3D& operator+=(const Vector3D& rhs);
	Vector3D operator*(float _scalar);
	Vector3D operator/(float _scalar);

	float getX() const;
	float getY() const;
	float getZ() const;
	value getValue() const;
	float distance(const Vector3D& rhs);
private:
	float length();
	value v;
};

value ptom(value& v);