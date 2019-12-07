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
#include "ObjectManager.h"
using namespace std;
Ship::Ship(){
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
	hp = 100;
	damage = 10;
	bulletCount = 3;
	pushType = E_NONE;
	maxSpeed = 15.0f;
	gearTime = 0.0f;
	direction = Vector3D(1.0f, 0.0f, 0.0f);
	rad = 0.0f;
}

void Ship::update(float deltaTime){
	for (auto i : collision->getCollisionObject()) {
		if (i->getType() == E_BULLET) {
			auto it = getObjectCast<Bullet>(i);
			if(shipIdx != it->getShipIdx() )
				i->setDelete();
		}
		else if (i->getType() == E_ITEM) {
			i->setDelete();
		}
		else if (i->getType() == E_REEF) {
			value rpos = i->getPos() - getPos();
			float degree = fabs(radToDegree(atan2(direction.getY(), direction.getX())) - radToDegree(atan2(rpos.y, rpos.x)));
			if (degree <= 45)
				setVelocity(0.f, 0.f, 0.f);
		}
		else if (i->getType() == E_SHIP) {
			auto sship = getObjectCast<Ship>(i);
			setVelocity(0.f, 0.f, 0.f);
			sship->setVelocity(0.f, 0.f, 0.f);
		}
	}
	if (pushType != E_NONE) {
		gearTime += deltaTime;
		if (gearTime > 0.2f) {
			if (pushType == E_PUSH)
				increasedSpeed();
			else
				decreasedSpeed();
			Vector3D v = getVelocity();
			v.setX(clamp(-maxSpeed, v.getX(), maxSpeed));
			v.setY(clamp(-maxSpeed, v.getY(), maxSpeed));
			setVelocity(v);
			gearTime = 0.0f;
		}
	}
	if (bulletCount < 3) {
		coolTime += deltaTime;;
		if (coolTime > 1.f) {
			bulletCount++;
			coolTime = 0.0f;
		}
	}
	Object::update(deltaTime);
	
	Vector3D v = getPos();
	v.setX(clamp(-700.0f, v.getX(), 700.0f));
	setPos(v.getX(), v.getY(), v.getZ());
}

void Ship::increasedSpeed(){
	Vector3D velocity = getVelocity();
	velocity += direction;
	setVelocity(velocity);
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
		velocity.setX((x > 0.9f) ? x : (x < -0.9f) ? x : 0.0f);
		velocity.setY((y > 0.9f) ? y : (y < -0.9f) ? y : 0.0f);
	}
	setVelocity(velocity);
}

void Ship::rotation(float f){

	Vector3D v = getVelocity();
	v.setX((std::cos(f) * v.getX()) - (std::sin(f) * v.getY()));
	v.setY(std::sin(f) * v.getX() + std::cos(f) * v.getY());
	setVelocity(v);

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

void Ship::addSpeed(float f) {
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
	if (hp < 1)
		setLive(false);
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

int Ship::getHp()
{
    return hp;
}

void Ship::setHp(int hp)
{
	this->hp = hp;
}

Vector3D Ship::getDirection()
{
	return direction;
}

void Ship::setDirection(value v){
	direction = v;
}

void Ship::setRadian(float r){
	rad = r;
}
