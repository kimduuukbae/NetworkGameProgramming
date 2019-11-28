#pragma once
#include "Object.h"

class Wind : public Object
{
public:
	Wind();
	~Wind() = default;
	void update(float deltaTime) override;
	void collideWind(Object* obj);
};

