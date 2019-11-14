#include "stdafx.h"
#include "ship.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
Ship::Ship(){
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
	maxSpeed = 7.0f;
	pushType = E_NONE;
	direction = Vector3D(0.0f, 1.0f, 0.0f);
}

void Ship::update(float deltaTime){
	Object::update(deltaTime);
	if (collision->getCollisionObject().size() != 0) 
		collision->getCollisionObject().back()->setDelete();
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
	if (gearTime > 1.0f) {
		Vector3D velocity = getVelocity();
		velocity += -direction;
		setVelocity(velocity);
		gearTime = 0.0f;
		if (velocity.size() < 0.1f)
			pushType = E_NONE;
	}
}

void Ship::increaseSpeed(){
	if (gearTime > 1.0f) {
		Vector3D velocity = getVelocity();
		velocity += direction;
		setVelocity(velocity);
		gearTime = 0.0f;
	}
	
}

void Ship::changePushType(E_PUSHTYPE e){
	pushType = e;
}
