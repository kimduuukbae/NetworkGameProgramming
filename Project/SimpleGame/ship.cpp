#include "stdafx.h"
#include "ship.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Item.h"
#include "Reef.h"
#include "bullet.h"
#include "Wind.h"
#include <iostream>
#include <cmath>
using namespace std;
Ship::Ship(){
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
	maxSpeed = 1.0f;
	hp = 100;
	damage = 10;
	bulletCount = 10;
	pushType = E_NONE;
	gearTime = 0.0f;
	direction = Vector3D(1.0f, 0.0f, 0.0f);
	rad = 0.0f;
}

void Ship::update(float deltaTime){
	if (pushType != E_NONE) {
		gearTime += deltaTime;
		if (gearTime > 1.0f) {
			if (pushType == E_PUSH)
				increasedSpeed();
			else
				decreasedSpeed();
			gearTime = 0.0f;
		}
	}
	if (bulletCount < 10) {
		coolTime += deltaTime;;
		if (coolTime > 1.f) {
			bulletCount++;
			coolTime = 0.0f;
		}
	}
	Object::update(deltaTime);
}

void Ship::increasedSpeed(){
	Vector3D velocity = getVelocity();
	velocity += direction;
	setVelocity(velocity);
	cout << "속도 증가 중" << velocity.getX() << "   " << velocity.getY() << endl;
}

void Ship::decreasedSpeed(){
	Vector3D velocity = getVelocity();
	if (velocity.size() < 0.1f) {
		pushType = E_NONE;
		velocity.setX(0.0f);
		velocity.setY(0.0f);
	}
	else {
		Vector3D v = velocity;
		v.normalize();
		velocity += -v;
		auto[x, y, z] = velocity.getValue();
		velocity.setX((x > 1.0f) ? x : (x < -1.0f) ? x : 0.0f);
		velocity.setY((y > 1.0f) ? y : (y < -1.0f) ? y : 0.0f);
		setVelocity(velocity);
		cout << "속도 감소 중" << velocity.getX() << "   " << velocity.getY() << endl;
	}
}

void Ship::rotation(float f){
	rad += f;
	if (rad < -12.56f || rad > 12.56f)
		rad = 0.0f;
	float x = direction.getX();
	float y = direction.getY();
	x = std::cos(rad);
	y = std::sin(rad);
	direction = Vector3D{ x,y,0.0f };
	setDegree(getDegree() + radToDegree(f));
}

void Ship::addSpeed(float f){
	if (f > 0.0f) 
		pushType = E_PUSH;
	else 
		pushType = E_RELEASED;
}


void Ship::changePushType(E_PUSHTYPE e){
	pushType = e;
}

E_PUSHTYPE Ship::getPushType(){
	return pushType;
}

void Ship::manageHp(int damage){
	hp -= damage;
	if (hp > 100)
		hp = 100;
}

void Ship::setMaxSpeed(float speed){
	maxSpeed = speed;
}

float Ship::getMaxSpeed(){
	return maxSpeed;
}

void Ship::setDamage(int damage){
	this->damage = damage;
}

int Ship::getDamage(){
	return damage;
}

void Ship::setShipIdx(int idx){
	shipIdx = idx;
}

int Ship::getShipIdx(){
	return shipIdx;
}

void Ship::setbulletCooltime(int count){
	bulletCount = count;
}

int Ship::getbulletCooltime(){
	return bulletCount;
}
