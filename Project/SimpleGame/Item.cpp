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
	case 1:	// ���� ü�� ����
		ship->manageHp(20);
		break;
	case 2:	// ���� �ӵ� ����
	{
		value v = ship->getVelocity();
		ship->setVelocity(1.5f * v.x, 1.5f * v.y, 1.5f + v.z);
	}
		break;
	case 3:	// ���� ���ݷ� ����
		break;
	}
}
