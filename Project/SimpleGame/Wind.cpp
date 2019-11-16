#include "stdafx.h"
#include "Wind.h"
#include "CollisionComponent.h"

Wind::Wind()
{
	addComponent<ICollisionComponent>();
}

void Wind::setWind(Vector3D w)
{
	wind = w;
}

void Wind::collideWind(Object* obj)
{
	Vector3D vel = obj->getVelocity();
	obj->setVelocity(vel.getX() + wind.getX, vel.getY() + wind.getY, vel.getZ() + wind.getZ);
}
