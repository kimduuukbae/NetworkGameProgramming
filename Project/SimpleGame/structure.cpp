#include "stdafx.h"
#include "structure.h"
#include <cmath>
value::value() :x{ 0 }, y{ 0 }, z{ 0 }{}
value::value(int){
	x = 0, y = 0, z = 0;
}
value::value(float tx, float ty, float tz) : x{ tx }, y{ ty }, z{ tz }{}
value value::operator*(float scalar) {
	return value{ x*scalar, y*scalar, z*scalar };
}
value value::operator+(const value& other) {
	return value{ x + other.x, y + other.y , z + other.z };
}
bool value::operator<(float scalar) {
	float F{ x + y + z };
	return F < scalar;
}

color::color() :r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 }{}

color::color(float r1, float g1, float b1, float a1) : r{ r1 }, g{ g1 }, b{ b1 }, a{ a1 }{}
Vector3D::Vector3D(float x, float y, float z) : v{ x,y,z } {}

Vector3D::Vector3D(value tv) : v { tv }{}
Vector3D& Vector3D::normalize(){
	float len = length();
	v.x /= len;
	v.y /= len;
	v.z /= len;
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D & rhs){
	return Vector3D{ rhs.v.x - v.x, rhs.v.y - v.y, rhs.v.z - v.z };
}

Vector3D & Vector3D::operator=(const Vector3D & rhs){
	v = rhs.v;
	return *this;
}

Vector3D Vector3D::operator+(const Vector3D & rhs){
	return { rhs.v.x + v.x, rhs.v.y + v.y, rhs.v.z + v.z };
}

Vector3D & Vector3D::operator+=(const Vector3D & rhs){
	v = rhs.v;
	return *this;
}

Vector3D Vector3D::operator*(float _scalar){
	return Vector3D{ v * _scalar };
}


float Vector3D::distance(const Vector3D & rhs){
	return Vector3D{ v.x - rhs.v.x , v.y - rhs.v.y, v.z - rhs.v.z }.length();
}

float Vector3D::getX() const {
	return v.x;
}

float Vector3D::getY() const {
	return v.y;
}

float Vector3D::getZ() const {
	return v.z;
}

value Vector3D::getValue() const {
	return v;
}

float Vector3D::length(){
	return std::sqrtf(v.x* v.x + v.y*v.y + v.z*v.z);
}

value ptom(value& v) {		 // pixel to meter 
	return value{ v.x * 100, v.y * 100, v.z * 100 };
}