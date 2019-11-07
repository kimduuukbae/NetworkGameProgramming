#include "stdafx.h"
#include "testObject.h"
#include "PhysicsComponent.h"
#include "Input.h"
testObject::testObject() {
	physics = new IPhysicsComponent();
}

void testObject::update(float deltaTime){
	Object::update(deltaTime);
	float fx = 0.0f, fy = 0.0f, fz = 0.0f;
	if (D_INPUT->isKeyDown(VK_LEFT))
		fx -= 5000.0f;
	if (D_INPUT->isKeyDown(VK_RIGHT))
		fx += 5000.0f;
	if (D_INPUT->isKeyDown(VK_UP))
		fy += 5000.0f;
	if (D_INPUT->isKeyDown(VK_DOWN))
		fy -= 5000.0f;

	physics->addForce(fx, fy, fz);
	physics->process(this);
}
