#pragma once
#include "Structure.h"
enum E_OBJECT_TYPE {
	E_SHIP,
	E_BULLET,
	E_ITEM,
	E_REEF
};
enum E_SHIP_PUSH_TYPE {
	E_PUSH,
	E_NONE,
	E_RELEASED
};
class Object {
public:
	Object() = delete;
	Object(value pos, value dir, E_OBJECT_TYPE e);
	~Object() = default;
	void update(double deltaTime);
	void increaseSpeed();
	void decreaseSpeed();
	void rotation(float r);
	void addSpeed(float f);
	
	E_OBJECT_TYPE getType();
	value getPos();
	value getVelocity();
private:
	Vector3D position;
	Vector3D direction;
	Vector3D velocity;
	E_OBJECT_TYPE type;
	E_SHIP_PUSH_TYPE pushType;
	int idx;

	float rad;

	double gearTime;
};