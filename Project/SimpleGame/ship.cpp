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
		// setDelete 함수아시죠? 얘를 지워줄꺼면 setDelete 함수 호출해주면 자동으로 지워집니다.
		// 이것도 menuScene과 똑같이 충돌된 오브젝트를 꺼내오는 용도입니다.
	}
}
