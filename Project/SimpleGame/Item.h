#pragma once
#include "ship.h"

class Item : public Object
{
	int effect;
public:
	Item();
	~Item() = default;
	virtual void applyEffect(Ship* ship) = 0;
};


class SpeedItem : public Item {
public:
	SpeedItem() = default;
	~SpeedItem() = default;
	void applyEffect(Ship* ship) override;
};