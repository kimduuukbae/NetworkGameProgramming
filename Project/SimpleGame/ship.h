#pragma once
#include "Object.h"
class ICollisionComponent;
class Ship : public Object {
public:
	Ship();
	~Ship() = default;
	void update(float deltaTime) override;

private:
	ICollisionComponent* collision;
};