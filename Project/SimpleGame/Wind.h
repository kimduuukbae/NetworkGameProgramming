#pragma once
#include "Object.h"

class Wind : public Object
{
public:
	Wind() = default;
	~Wind() = default;
	void update(float deltaTime) override;
};

