#pragma once
#include "Object.h"
class IPhysicsComponent;

class testObject : public Object {

public:
	testObject();
	~testObject() = default;
	void update(float deltaTime) override;
private:
	IPhysicsComponent* physics;
};
