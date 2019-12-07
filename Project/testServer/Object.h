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
	void update(float deltaTime);
	void increaseSpeed();
	void decreaseSpeed();
	void rotation(float r);
	void addSpeed(float f);
	
	void setIdx(int i);
	void setAncesterIdx(int i);
    void manageHp(int damage);
    int getHp();
	void setMaxSpeed(float maxSpeed);
	float getMaxSpeed();
	void setDamage(int damage);
	int getDamage();
	inline int getAncester() { return ancesteridx; }
	inline int getIdx() { return idx; }

	void setDelete();
	inline bool getDelete() { return deleteIt; }

	box getBox();
	inline E_OBJECT_TYPE getType() { return type; }

	void setPos(value pos);
	inline value getPos() { return position.getValue(); }

	void setVelocity(float x, float y, float z);
	inline value getVelocity() { return velocity.getValue(); }

	void setDirection(float x, float y, float z);
	inline value getDirection(){ return direction.getValue(); }

	void setLive(bool flag);
	inline bool getLive() { return live; }
	
	void setCollobject(bool flag);
	bool getCollobject();

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
	float maxSpeed;
	int damage;
	bool deleteIt;
	bool live;
	bool coll{ false };
};