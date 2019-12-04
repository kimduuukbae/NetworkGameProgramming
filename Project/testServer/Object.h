#pragma once
#include "Structure.h"
enum E_OBJECT_TYPE {
	E_SHIP,
	E_BULLET,
	E_ITEM,
	E_REEF,
	E_WIND
};
enum E_SHIP_PUSH_TYPE {
	E_PUSH,
	E_NONE,
	E_RELEASED
};
class Object {
public:
	Object() = delete;
	Object(value pos, value dir, value volume,  E_OBJECT_TYPE e);
	~Object() = default;
	void update(double deltaTime);
	void increaseSpeed();
	void decreaseSpeed();
	void rotation(float r);
	void addSpeed(float f);
	void setVelocity(float x, float y, float z);
	void setIdx(int i);
	void setAncesterIdx(int i);
    void manageHp(int damage);
    int getHp();
	int getAncester();
	int getIdx();

	void setDelete();
	bool getDelete();
	box getBox();
	E_OBJECT_TYPE getType();
	value getPos();
	void setPos(value pos);
	value getVelocity();
private:
	Vector3D position;
	Vector3D direction;
	Vector3D velocity;
	E_OBJECT_TYPE type;
	E_SHIP_PUSH_TYPE pushType;

	box volume;

	int idx;
	float rad;
	double gearTime;
	int ancesteridx;
    int HealthPoint;
	
	bool deleteIt;
};