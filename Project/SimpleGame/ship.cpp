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
	pushType = E_NONE;
	direction = Vector3D(1.0f, 0.0f, 0.0f);
	degree = 0.0f;
}

void Ship::update(float deltaTime){
	if (pushType == E_PUSH) {
		gearTime += deltaTime;
		increaseSpeed();
	}
	else if (pushType == E_RELEASED) {
		gearTime += deltaTime;
		decreaseSpeed();
	}
	for (auto& i : collision->getCollisionObject()) {
		// collision->getCollisionObject() �� ���� ���� �浹�� �༮����
		// std::list<Object*> �������� �����ϴ�.
		// �׷��Ƿ� size �� 0�϶��� �� ������ ������ �ʰ���? �浹�� �༮�� ��������
		// �� range for loop �� �۵��մϴ�
		if (i->getType() == E_ITEM) {
			auto tmp = getObjectCast<Item>(i);
			tmp->applyEffect(this);
		}
		else if (i->getType() == E_BULLET) {
			auto tmp = getObjectCast<Bullet>(i);
			if (tmp->getShipIdx() != shipIdx) {
				i->setDelete();
				this->setDelete();
			}
		}
		else if (i->getType() == E_REEF) {
			auto tmp = getObjectCast<Reef>(i);
			//tmp->collideReef(this);
		}
		else if (i->getType() == E_WIND) {
			auto tmp = getObjectCast<Wind>(i);
			tmp->collideWind(this);
		}
		// �浹 üũ�� �̿Ͱ��� ����Ͻø� �˴ϴ�.
		// getObjectCast<T> �� Object.h�� ����� �ξ�����
		// � Object* �� �ڽ��� ��¥ Derived class �� ��������ݴϴ�!
		// �� : object* -> Item
	}
	Object::update(deltaTime);
}

void Ship::decreaseSpeed(){
	if (gearTime > 0.1f) {
		Vector3D velocity = getVelocity();
		velocity += -direction;
		gearTime = 0.0f;
		velocity.setX(velocity.getX() > 0.0f ? velocity.getX() : 0.0f);
		velocity.setY(velocity.getY() > 0.0f ? velocity.getY() : 0.0f);
		if (velocity.size() < 0.1f) {
			pushType = E_NONE;
			velocity.setX(0.0f);
			velocity.setY(0.0f);
		}
		setVelocity(velocity);
	}
}

void Ship::increaseSpeed(){
	if (gearTime > 0.2f) {
		Vector3D velocity = getVelocity();
		velocity += direction;
		setVelocity(velocity);
		gearTime = 0.0f;
		cout << getVelocity().x << endl;
	}
	
}

void Ship::leftRotation(){
	degree -= 0.01f;
	if (degree < -6.28f || degree > 6.25f)
		degree = 0.0f;
	float x = direction.getX();
	float y = direction.getY();
	x = std::cos(degree);
	y = std::sin(degree);
	direction = Vector3D{ x,y,0.0f };
}

void Ship::rightRotation(){

}

void Ship::changePushType(E_PUSHTYPE e){
	pushType = e;
}

void Ship::manageHp(int damage){
	hp -= damage;
	if (hp > 100)
		hp = 100;
}

void Ship::setMaxSpeed(float speed){
	maxSpeed = speed;
}

float Ship::getMaxSpeed()
{
	return maxSpeed;
}

void Ship::setDamage(int damage)
{
	this->damage = damage;
}

int Ship::getDamage()
{
	return damage;
}

void Ship::setShipIdx(int idx){
	shipIdx = idx;
}

int Ship::getShipIdx(){
	return shipIdx;
}
