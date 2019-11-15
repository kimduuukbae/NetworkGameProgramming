#include "stdafx.h"
#include "Reef.h"
#include "CollisionComponent.h"

Reef::Reef()
{
	addComponent<ICollisionComponent>();
}

void Reef::collideReef(Object* obj)
{
	obj->setVelocity(0, 0, 0);
}
