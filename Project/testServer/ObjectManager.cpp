#include "ObjectManager.h"
#include "Structure.h"
#include <iostream>
int ObjectManager::addObject(value pos, value dir, value v,  E_OBJECT_TYPE e){
	objects.emplace_back(pos, dir, v, e);
	objects.back().setIdx(objects.size() - 1);
	return objects.size()-1;
}

void ObjectManager::update(double deltaTime){
	for (auto& i : objects) {
		i.update(deltaTime);

		if (i.getType() == E_WIND)
			continue;

		for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
			if (&i == &(*it) || (i.getType() == E_BULLET & i.getAncester() == (*it).getIdx()))
				continue;
			if (AABBCollision((*it).getBox(), i.getBox())) {
				std::cout << "어떤 놈 둘이 충돌함!" << std::endl;
			}
		}
	}
}
