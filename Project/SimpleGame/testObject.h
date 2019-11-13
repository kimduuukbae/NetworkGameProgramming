#pragma once
#pragma once
#include "Object.h"
class ICollisionComponent;

class test : public Object {
public:
	test();
	~test() = default;
	void update(float deltaTime) override;

private:
	ICollisionComponent* collision;
};