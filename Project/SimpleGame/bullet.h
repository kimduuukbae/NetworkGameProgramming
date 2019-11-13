#pragma once
#include "Object.h"
class ICollisionComponent;
class Bullet : public Object {
public:
	Bullet();
	~Bullet() = default;
	void update(float deltaTime) override;

private:
	ICollisionComponent* collision;
};

