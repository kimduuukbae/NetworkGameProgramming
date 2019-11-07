#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Object.h"
#include "Input.h"
#include <iostream>
using namespace std;
void IPhysicsComponent::process(Object* o){
	Vector3D velocity{ o->getVelocity() };
	Vector3D acc{ power / o->getMass() };

	velocity = velocity + acc * Input::instance()->getDeltaTime();
	auto [x,y,z] = o->getPos() + velocity.getValue() *  Input::instance()->getDeltaTime();
	o->setPos(x, y, z);
}

void IPhysicsComponent::addForce(float x, float y, float z){
	power = Vector3D{ x,y,z };
}
