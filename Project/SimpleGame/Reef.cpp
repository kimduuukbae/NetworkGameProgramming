#include "stdafx.h"
#include "Reef.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"

Reef::Reef()
{
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Reef::update(float deltaTime)
{
	for (auto i : collision->getCollisionObject())
		if (i->getType() == E_ITEM) {
			i->setVelocity(0.f, 0.f, 0.f);
		}
}
