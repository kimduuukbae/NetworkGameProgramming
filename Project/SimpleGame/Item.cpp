#include "stdafx.h"
#include "Item.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ship.h"

Item::Item()
{
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	effect = rand() % 3;
}

void Item::applyEffect(Ship* ship)
{
	switch (effect) {
	case 1:	// 배의 체력 증가
		ship->manageHp(20);
		break;
	case 2:	// 배의 속도 증가
	{
		value v = ship->getVelocity();
		ship->setVelocity(1.5f * v.x, 1.5f * v.y, 1.5f + v.z);
	}
		break;
	case 3:	// 배의 공격력 증가
		break;
	}
}
