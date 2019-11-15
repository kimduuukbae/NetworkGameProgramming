#pragma once
#include "Object.h"
class ICollisionComponent;

enum E_PUSHTYPE {
	E_NONE,
	E_PUSH,
	E_RELEASED
};

class Ship : public Object {
public:
	Ship();
	~Ship() = default;
	void update(float deltaTime) override;
	void decreaseSpeed();
	void increaseSpeed();
	void changePushType(E_PUSHTYPE e);
	void manageHp(int damage);
	void setMaxSpeed();
	void setShipIdx(int idx);
	int getShipIdx();
private:
	ICollisionComponent* collision;
	float gearTime;
	float maxSpeed;
	int hp;
	int shipIdx;
	E_PUSHTYPE pushType;
	Vector3D direction;
};