#include "stdafx.h"
#include "Reef.h"
#include "Item.h"
#include "bullet.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"

Reef::Reef()
{
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Reef::update(float deltaTime)
{
	Object::update(deltaTime);
	for (auto i : collision->getCollisionObject())
		if (i->getType() == E_ITEM) {
			i->setCollobject(true);
		}
		else if (i->getType() == E_BULLET) {
			i->setDelete();
		}
}
