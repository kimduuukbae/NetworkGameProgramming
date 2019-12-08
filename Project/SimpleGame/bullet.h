#pragma once
#include "Object.h"
class ICollisionComponent;
class Bullet : public Object {
public:
	Bullet();
	~Bullet() = default;
	void update(float deltaTime) override;
	void process(short mx, short my, short sPosX, short sPosY);
	void setShipIdx(int idx);
	int getShipIdx();
	void setvTime(float time);
	int getvTime();

private:
	int shipIdx;
	float vTime;
	float fTime = 0.0f;
	value bVol;
	ICollisionComponent* collision;
};

