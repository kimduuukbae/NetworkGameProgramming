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

	if (fTime < vTime / 2) {
		bVol.x += 0.1f; bVol.y += 0.1f;
		setVolume(bVol.x, bVol.y, 0.0f);
	}
	else {
		bVol.x -= 0.1f; bVol.y -= 0.1f;
		setVolume(bVol.x, bVol.y, 0.0f);
	}

	fTime += deltaTime;
	if (vTime < fTime)
		setDelete();
}

void Bullet::process(short mx, short my, short sPosX, short sPosY) {
	vTime = sqrtf(pow(mx - sPosX, 2) + pow(my - sPosY, 2)) / 300.f;
	
	setVelocity((mx - sPosX) / vTime, (my - sPosY) / vTime, 0.0f);
	bVol = getVolume();
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
