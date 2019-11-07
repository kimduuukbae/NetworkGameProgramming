#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Object.h"
#include "Input.h"

void IPhysicsComponent::process(Object* o){
	Vector3D velocity{ o->getVelocity() };
	Vector3D acc{ power / o->getMass() };
	float time = D_INPUT->getDeltaTime();

	velocity = velocity + acc * time;

	auto[vx, vy, vz] = velocity.getValue();
	auto [x,y,z] = o->getPos() + velocity.getValue() *  time;

	o->setPos(x, y, z);
	o->setVelocity(vx, vy, vz);
}

void IPhysicsComponent::addForce(float x, float y, float z){
	power = Vector3D{ x,y,z };
}
