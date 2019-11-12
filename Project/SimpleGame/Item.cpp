#include "stdafx.h"
#include "Item.h"
#include "PhysicsComponent.h"
#include "structure.h"

Item::Item()
{
	addComponent<IPhysicsComponent>();
	effect = rand() % 3;
}

void Item::applyEffect(Object* obj)
{
	switch (effect) {
	case 1:	// ���� ü�� ����
		break;
	case 2:	// ���� �ӵ� ����
		value v = obj->getVelocity();
		obj->setVelocity(1.5f * v.x, 1.5f * v.y, 1.5 * v.z);
		break;
	case 3:	// ���� ���ݷ� ����
		break;
	}
}
