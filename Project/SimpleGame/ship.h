#pragma once
#include "Object.h"

class Ship : public Object {
public:
	Ship();
	~Ship() = default;
	void update(float deltaTime) override;

private:
};