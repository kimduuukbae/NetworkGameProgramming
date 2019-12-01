#include "ObjectManager.h"
#include "Structure.h"
#include <iostream>
#include "ServerDevice.h"
#include "EventManager.h"

ObjectManager::ObjectManager(){
	eventManager = EventManager::instance();
}

int ObjectManager::addObject(value pos, value dir, value v,  E_OBJECT_TYPE e){
	objects.emplace_back(pos, dir, v, e);
	objects.back().setIdx(objects.size() - 1);
	return objects.size()-1;
}

void ObjectManager::update(double deltaTime){
	for (auto& i : objects) {
		if (i.getType() == E_WIND)
			continue;

		for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
			if (&i == &(*it) || (i.getType() == E_BULLET & i.getAncester() == (*it).getIdx()))
				continue;
			if (AABBCollision((*it).getBox(), i.getBox())) {
				std::cout << "어떤 놈 둘이 충돌함!" << std::endl;
			}
		}
		i.update(deltaTime);
	}
	ItemCreateTime -= deltaTime;
	if (ItemCreateTime < FLT_EPSILON)
	{
		ItemCreateTime = 20.f;
		short effect = rand() % 3;
		short posX = rand() % (775 - (-775) + 1) + (-775);
		short posY = rand() % (425 - (-425) + 1) + (-425);
		eventManager->pushEvent(itemPacket{ effect,posX,posY }, E_SEND);
	}
	windChangeTime -= deltaTime;
	if (windChangeTime < FLT_EPSILON)
	{
		windChangeTime = 30.f;
		short wind = -1;
		short windVelX = rand() % (10 - (-10) + 1) + (-10);
		short windVelY = rand() % (10 - (-10) + 1) + (-10);
		eventManager->pushEvent(itemPacket{ wind,windVelX,windVelY }, E_SEND);
	}
}
