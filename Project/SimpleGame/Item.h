#pragma once
#include "ship.h"

class Item : public Object
{
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

class DamageItem : public Item
{
public:
	DamageItem() = default;
	~DamageItem() = default;
	void applyEffect(Ship* ship) override;
};

class HealItem : public Item
{
public:
	HealItem() = default;
	~HealItem() = default;
	void applyEffect(Ship* ship) override;
};