#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Object.h"
#include "Input.h"

void IPhysicsComponent::process(Object* o){
	Vector3D velocity{ o->getVelocity() };
	float time = D_INPUT->getDeltaTime();

	auto [x,y,z] = o->getPos() + velocity.getValue() *  time;

	o->setPos(x, y, z);
}
