#pragma once
#include <vector>
#include "Object.h"

class ObjectManager {
public:
	ObjectManager() = default;
	void addObject(value pos, value dir,E_OBJECT_TYPE e);
	void update(double deltaTime);
	Object& findObject(int idx);
private:
	std::vector<Object> objects;
};