#pragma once
#include "Object.h"
class IPhysicsComponent;

class Ship : public Object {
public:
	Ship();
	~Ship() = default;
	void update(float deltaTime) override;

private:
	IPhysicsComponent* physics;
};