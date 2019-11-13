#pragma once
#include "Object.h"


class Item : public Object
{
	int effect;
public:
	Item();
	~Item() = default;
	void applyEffect(Object* obj);
};

