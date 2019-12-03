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

	void increasedSpeed();
	void decreasedSpeed();

	void rotation(float f);
	void addSpeed(float f);
	void changePushType(E_PUSHTYPE e);
	E_PUSHTYPE getPushType();
	void manageHp(int damage);
	void setMaxSpeed(float speed);
	float getMaxSpeed();
	void setDamage(int damage);
	int getDamage();
	void setShipIdx(int idx);
	int getShipIdx();
	void setbulletCooltime(int count);
	int getbulletCooltime();
    int getHp();
	void setHp(int hp);
private:
	ICollisionComponent* collision;
	float gearTime;
	float coolTime;
	float maxSpeed;
	int hp;
	int damage;
	int shipIdx;
	int bulletCount;
	float degree;
	float rad;
	E_PUSHTYPE pushType;
	Vector3D direction;
};