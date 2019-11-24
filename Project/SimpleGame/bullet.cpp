#include "stdafx.h"
#include "bullet.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
#include "ship.h"
#include<cmath>
float fx, fy;
Bullet::Bullet() {
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Bullet::update(float deltaTime) {
	Object::update(deltaTime);
	fTime += deltaTime;
	if (vTime < fTime)
		setDelete();
}

void Bullet::process(short mx, short my, short sPosX, short sPosY) {
		vTime = sqrtf((mx - sPosX) * (mx - sPosX) + (my - sPosY) * (my - sPosY)) / 300.f;

		if (vTime < fTime) {
			setVelocity(0.0f, 0.0f, 0.0f);
		}
		else
			setVelocity((mx - sPosX) / vTime, (my - sPosY) / -vTime, 0.0f);
		setPos(sPosX, sPosY, 0.0f);
}

void Bullet::setShipIdx(int idx){
	shipIdx = idx;
}

int Bullet::getShipIdx(){
	return shipIdx;
}

void Bullet::setvTime(float time)
{
	vTime = time;
}

int Bullet::getvTime()
{
	return vTime;
}
