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
	ship->setMaxSpeed(ship->getMaxSpeed() + 5.f);
}

void DamageItem::applyEffect(Ship* ship)
{
	ship->setDamage(ship->getDamage() + 5);
}

void HealItem::applyEffect(Ship* ship)
{
	ship->manageHp(-20);
}
