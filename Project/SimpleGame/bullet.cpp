#include "stdafx.h"
#include "bullet.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
#include "ship.h"
#include<cmath>

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
	short fx = mx - 800;
	short fy = my - 450;

	vTime = sqrtf(pow(fx - sPosX, 2) + pow(fy - sPosY, 2)) / 300.f;

	setVelocity((fx - sPosX) / vTime, (fy - sPosY) / vTime, 0.0f);
	printf("%d %d", fx, fy);
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
