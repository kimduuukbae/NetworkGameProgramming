#pragma once
#include "Object.h"

class ICollisionComponent;
class Reef : public Object
{
public:
	Reef();
	~Reef() = default;
	void update(float deltaTime) override;
private:
	ICollisionComponent* collision;
};

