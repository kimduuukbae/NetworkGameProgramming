#pragma once
#include "Object.h"

class Reef : public Object
{
public:
	Reef();
	~Reef() = default;
	void collideReef(Object* obj);
};

