#include "stdafx.h"
#include "bullet.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
#include<iostream>
#include<cmath>
Bullet::Bullet() {
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void Bullet::update(float deltaTime) {
	Object::update(deltaTime);
	float fx = 0.0f, fy = 0.0f, fz = 0.0f;
	
	if (D_INPUT->isMouseDown()) {
		fx = D_INPUT->mx - 512;
		fy = -D_INPUT->my + 512;

		printf("%f, %f\n", fx, fy);

		//setPos(fx, fy, fz);

		struct value v = getPos();

		std::cout << v.x << ", " << v.y << std::endl;

		/*float dirX = 0.0f, dirY = 0.0f;

		if (fx - v.x > 0 && fy - v.y > 0) {
			dirX = 1.f; dirY = 1.f;
		}
		if (fx - v.x < 0 && fy - v.y > 0) {
			dirX = -1.f; dirY = 1.f;
		}
		if (fx - v.x < 0 && fy - v.y < 0) {
			dirX = -1.f; dirY = -1.f;
		}
		if (fx - v.x > 0 && fy - v.y < 0) {
			dirX = 1.f; dirY = -1.f;
		}*/

		float vTime = sqrt((fx - v.x) * (fx - v.x) + (fy - v.y) * (fy - v.y)) / 300;

		float ax = 0.0f, ay = 0.0f, az = 0.0f;
		ax =  ((fx - v.x) / vTime);
		ay =  ((fy - v.y) / vTime);

		std::cout << ax << ", " << ay << std::endl;

		setVelocity(ax, ay, az);
	}

	if (collision->getCollisionObject().size() != 0) {
		collision->getCollisionObject().back()->setDelete();
		// setDelete �Լ��ƽ���? �긦 �����ٲ��� setDelete �Լ� ȣ�����ָ� �ڵ����� �������ϴ�.
		// �̰͵� menuScene�� �Ȱ��� �浹�� ������Ʈ�� �������� �뵵�Դϴ�.
	}
}
