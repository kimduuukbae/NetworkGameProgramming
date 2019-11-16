#include "stdafx.h"
#include "ship.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Item.h"
#include "Reef.h"
#include "bullet.h"
#include "Wind.h"
Ship::Ship(){
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
	maxSpeed = 7.0f;
	hp = 100;
	damage = 10;
	pushType = E_NONE;
	direction = Vector3D(0.0f, 1.0f, 0.0f);
}

void Ship::update(float deltaTime){
	Object::update(deltaTime);
	for (auto& i : collision->getCollisionObject()) {
		// collision->getCollisionObject() 는 현재 내가 충돌한 녀석들을
		// std::list<Object*> 형식으로 뱉어냅니다.
		// 그러므로 size 가 0일때는 이 루프는 돌지도 않겠죠? 충돌한 녀석이 있을때만
		// 이 range for loop 가 작동합니다
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
			tmp->collideReef(this);
		}
		else if (i->getType() == E_WIND) {
			auto tmp = getObjectCast<Wind>(i);
			tmp->collideWind(this);
		}
		// 충돌 체크는 이와같이 사용하시면 됩니다.
		// getObjectCast<T> 는 Object.h에 만들어 두었으며
		// 어떤 Object* 를 자신의 진짜 Derived class 로 변경시켜줍니다!
		// 예 : object* -> Item
	}
	
	if (pushType == E_PUSH) {
		gearTime += deltaTime;
		increaseSpeed();
	}
	else if (pushType == E_RELEASED) {
		gearTime += deltaTime;
		decreaseSpeed();
	}
}

void Ship::decreaseSpeed(){
	if (gearTime > 0.3f) {
		Vector3D velocity = getVelocity();
		velocity += -direction;
		setVelocity(velocity);
		gearTime = 0.0f;
		if (velocity.size() < 0.1f)
			pushType = E_NONE;
	}
}

void Ship::increaseSpeed(){
	if (gearTime > 0.0001f) {
		Vector3D velocity = getVelocity();
		velocity += direction;
		setVelocity(velocity);
		gearTime = 0.0f;
	}
	
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
