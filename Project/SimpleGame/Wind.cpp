#include "stdafx.h"
#include "Wind.h"
#include "CollisionComponent.h"

Wind::Wind()
{
	addComponent<ICollisionComponent>();
}

void Wind::update(float deltaTime)
{

}

void Wind::collideWind(Object* obj)
{
	value vel = obj->getVelocity();
	vel = vel + this->getVelocity();
	obj->setVelocity(Vector3D{ vel });
}
