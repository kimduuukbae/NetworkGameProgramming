#pragma once
#include <vector>
#include "Object.h"

class ObjectManager {
public:
	void addObject(value pos, value dir,E_OBJECT_TYPE e);
	void update();
	Object& findObject(int idx);
private:
	std::vector<Object> objects;
};