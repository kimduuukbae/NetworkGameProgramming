#pragma once
#include <vector>
#include "Object.h"

class ObjectManager {
public:
	ObjectManager() = default;
	int addObject(value pos, value dir, value v, E_OBJECT_TYPE e);
	void update(double deltaTime);
	inline Object& findObject(int idx) {
		return objects[idx];
	}
private:
	std::vector<Object> objects;
};