#include "stdafx.h"
#include "ship.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
Ship::Ship(){
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Ship::update(float deltaTime){
	Object::update(deltaTime);
	float fx = 0.0f, fy = 0.0f, fz = 0.0f;
	if (D_INPUT->isKeyDown(VK_LEFT))
		fx -= 50.0f;
	if (D_INPUT->isKeyDown(VK_RIGHT))
		fx += 50.0f;
	if (D_INPUT->isKeyDown(VK_UP))
		fy += 50.0f;
	if (D_INPUT->isKeyDown(VK_DOWN))
		fy -= 50.0f;
	setVelocity(fx, fy, fz);
	if (collision->getCollisionObject().size() != 0) {
		collision->getCollisionObject().back()->setDelete();
		// setDelete �Լ��ƽ���? �긦 �����ٲ��� setDelete �Լ� ȣ�����ָ� �ڵ����� �������ϴ�.
		// �̰͵� menuScene�� �Ȱ��� �浹�� ������Ʈ�� �������� �뵵�Դϴ�.
	}
}
