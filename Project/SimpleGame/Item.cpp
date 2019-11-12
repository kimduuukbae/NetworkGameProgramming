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
	case 1:	// 배의 체력 증가
		break;
	case 2:	// 배의 속도 증가
		value v = obj->getVelocity();
		obj->setVelocity(1.5f * v.x, 1.5f * v.y, 1.5 * v.z);
		break;
	case 3:	// 배의 공격력 증가
		break;
	}
}
