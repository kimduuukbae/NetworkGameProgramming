#pragma once
#include "ship.h"

class Item : public Object
{
	int effect;
public:
	Item();
	~Item() = default;
	void applyEffect(Ship* ship);
};

