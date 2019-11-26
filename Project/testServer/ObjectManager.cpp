#include "ObjectManager.h"

void ObjectManager::addObject(value pos, value dir, E_OBJECT_TYPE e){
	objects.emplace_back(pos, dir, e);
}

void ObjectManager::update(double deltaTime){
	for (auto& i : objects) {
		if (i.getType() == E_SHIP)
			i.update(deltaTime);
	}
}

Object & ObjectManager::findObject(int idx){
	return objects[idx];
}