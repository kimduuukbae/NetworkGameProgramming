#pragma once
#include <vector>
#include "Object.h"

class EventManager;

template <typename T>
void objectSwap(T& t1, T& t2) {
	Object& tmp = t1;
	t1 = t2;
	t2 = tmp;
}

class ObjectManager {
public:
	ObjectManager();
	int addObject(value pos, value dir, value v, E_OBJECT_TYPE e);
	void update(float deltaTime);
	inline Object& findObject(int idx) {
		return objects[idx];
	}
private:
	std::vector<Object> objects;
	EventManager* eventManager;

	float ItemCreateTime;
	float windChangeTime;
	float garbageTime;
	void garbageColliection();
	float rTime;
	int live;


	// private member function

	void collideBulletToShip(Object& o, const std::vector<Object>::iterator& shp);
	void collideItemToShip(Object& o, const std::vector<Object>::iterator& shp);
	void collideReefToShip(Object& o, const std::vector<Object>::iterator& shp);
	void collideShipToShip(Object& o, const std::vector<Object>::iterator& shp);
};