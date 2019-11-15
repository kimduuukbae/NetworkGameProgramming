#include "stdafx.h"
#include "Item.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ship.h"

Item::Item()
{
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
}

void SpeedItem::applyEffect(Ship * ship){
	ship->setMaxSpeed();
}
