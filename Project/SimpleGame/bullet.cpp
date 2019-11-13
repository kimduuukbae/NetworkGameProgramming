#include "stdafx.h"
#include "bullet.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
Bullet::Bullet() {
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Bullet::update(float deltaTime) {
	Object::update(deltaTime);
	float fx = 0.0f, fy = 0.0f, fz = 0.0f;
	
	if (D_INPUT->isMouseDown) {
		fx = (2 / (float)1600) * x - 1;
		fy = (-2 / (float)900) * y + 1;
	}

	setVelocity(fx, fy, fz);
	if (collision->getCollisionObject().size() != 0) {
		collision->getCollisionObject().back()->setDelete();
		// setDelete �Լ��ƽ���? �긦 �����ٲ��� setDelete �Լ� ȣ�����ָ� �ڵ����� �������ϴ�.
		// �̰͵� menuScene�� �Ȱ��� �浹�� ������Ʈ�� �������� �뵵�Դϴ�.
	}
}
