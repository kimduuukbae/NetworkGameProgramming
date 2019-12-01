#pragma once
#include <vector>
#include "Object.h"

class EventManager;

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

	float ItemCreateTime = 20.f;
	float windChangeTime = 30.f;
};