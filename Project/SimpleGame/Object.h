#pragma once
#include "structure.h"

class Object {
public:
	Object() = default;
	virtual ~Object() = default;
	void setPos(float x, float y, float z);
	value getPos();

	void setVolume(float x, float y, float z);
	value getVolume();

	void setMass(float m);
	float getMass();

	void setColor(float r, float g, float b, float a);
	color getColor();

	void setVelocity(float x, float y, float z);
	value getVelocity();

	virtual void update(float deltaTime);
	void setIdx(int idx);

	int getIdx();
private:
	value position;
	value volume;
	value velocity;
	float mass;

	color stColor;
	void setValue(float x, float y, float z, value& v);

	int pngIdx;
};
template <typename T>
Object* addObject(const value& pos, const color& c, const value& volume,
	const value& velocity, float fric, float mass);