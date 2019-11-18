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
	//vTime += fTime;
	if (vTime < fTime)
		setDelete();
}

void Bullet::process(Object* o, float dt) {

	if (D_INPUT->isMouseDown()) {
		value v = o->getPos();

		fx = D_INPUT->mx - 800;
		fy = D_INPUT->my - 450;

		vTime = sqrtf((fx - v.x) * (fx - v.x) + (fy - v.y) * (fy - v.y)) / 300.f;


		printf("%f", vTime);

		if (vTime < fTime) {
			setVelocity(0.0f, 0.0f, 0.0f);
		}
		else
			setVelocity((fx - v.x) / vTime, (fy - v.y) / -vTime, 0.0f);
		setPos(v.x, v.y, 0.0f);
	}
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
