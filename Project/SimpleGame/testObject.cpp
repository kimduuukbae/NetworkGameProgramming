#include "stdafx.h"
#include "testObject.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Input.h"
test::test() {
	addComponent<IPhysicsComponent>();
	addComponent<ICollisionComponent>();
	collision = getComponent<ICollisionComponent>();
}

void test::update(float deltaTime) {
	Object::update(deltaTime);
}
