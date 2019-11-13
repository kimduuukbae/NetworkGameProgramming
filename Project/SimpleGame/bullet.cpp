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
		// setDelete 함수아시죠? 얘를 지워줄꺼면 setDelete 함수 호출해주면 자동으로 지워집니다.
		// 이것도 menuScene과 똑같이 충돌된 오브젝트를 꺼내오는 용도입니다.
	}
}
