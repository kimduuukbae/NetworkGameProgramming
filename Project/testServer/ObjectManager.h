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
	void update(double deltaTime);
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
	float rTime = 0.f;
	int live = 3;
};