#pragma once
#include "Structure.h"
enum E_OBJECT_TYPE {
	E_SHIP,
	E_BULLET,
	E_ITEM,
	E_REEF
};

class Object {
public:
	Object() = delete;
	Object(value pos, value dir, E_OBJECT_TYPE e);
	~Object() = default;
	void move();
	void increaseSpeed();
	void decreaseSpeed();
	void rotation(float r);
	E_OBJECT_TYPE getType();
private:
	Vector3D position;
	Vector3D direction;
	Vector3D velocity;
	E_OBJECT_TYPE type;
	int idx;

	float rad;
};