#include "stdafx.h"
#include "bullet.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
#include "ship.h"
#include<cmath>
#include<iostream>
Bullet::Bullet() {
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Bullet::update(float deltaTime) {
	Object::update(deltaTime);

	for (auto i : collision->getCollisionObject())
		if (i->getType() == E_REEF)
			setDelete();

	fTime += deltaTime;
	if (vTime < fTime)
		setDelete();
}

void Bullet::process(short mx, short my, short sPosX, short sPosY) {
	vTime = sqrtf(pow(mx - sPosX, 2) + pow(my - sPosY, 2)) / 300.f;
	
	setVelocity((mx - sPosX) / vTime, (my - sPosY) / vTime, 0.0f);
	auto [x,y,z] = getVelocity();
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
