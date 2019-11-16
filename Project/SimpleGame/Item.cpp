#include "stdafx.h"
#include "Item.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ship.h"
#include "Reef.h"
#include "Wind.h"

Item::Item()
{
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
}

void Item::update(float deltaTime)
{
	Object::update(deltaTime);
	for (auto& i : collision->getCollisionObject()) {
		if (i->getType() == E_REEF) {
			auto tmp = getObjectCast<Reef>(i);
			tmp->collideReef(this);
		}
		else if (i->getType() == E_WIND) {
			auto tmp = getObjectCast<Wind>(i);
			tmp->collideWind(this);
		}
	}
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
