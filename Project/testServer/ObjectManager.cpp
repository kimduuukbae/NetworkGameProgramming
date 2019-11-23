#include "ObjectManager.h"

void ObjectManager::addObject(value pos, value dir, E_OBJECT_TYPE e){
	objects.emplace_back(pos, dir, e);
}

void ObjectManager::update(){
	for (auto& i : objects) {
		if (i.getType() == E_SHIP)
			i.move();
	}
}

Object & ObjectManager::findObject(int idx){
	return objects[idx];
}
