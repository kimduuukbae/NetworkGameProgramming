#pragma once
#include <list>
#include "Object.h"

class ObjectManager {
public:
	void addObject(value pos, value dir,E_OBJECT_TYPE e);
	void update();
private:
	std::list<Object> objects;
};