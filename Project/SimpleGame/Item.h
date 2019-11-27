#pragma once
#include "ship.h"

class Item : public Object
{
	ICollisionComponent* collision;
public:
	Item();
	~Item() = default;
	void update(float deltaTime) override;
	virtual void applyEffect(Ship* ship) = 0;
};

class SpeedItem : public Item {
public:
	SpeedItem();
	~SpeedItem() = default;
	void applyEffect(Ship* ship) override;
};

class DamageItem : public Item
{
public:
	DamageItem();
	~DamageItem() = default;
	void applyEffect(Ship* ship) override;
};

class HealItem : public Item
{
public:
	HealItem();
	~HealItem() = default;
	void applyEffect(Ship* ship) override;
};