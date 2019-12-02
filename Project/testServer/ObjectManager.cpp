#include "ObjectManager.h"
#include "Structure.h"
#include <iostream>
#include "ServerDevice.h"
#include "EventManager.h"

ObjectManager::ObjectManager() : 
ItemCreateTime{ 20.0f },
windChangeTime{ 30.0f },
garbageTime {10.0f},
eventManager{ EventManager::instance() }
{}

int x = 0;
//0 1 2 배, 3 바람 4 ~ 총알 아이템
void ObjectManager::update(double deltaTime){
	for (auto& i : objects) {
		if (i.getType() == E_WIND | i.getDelete())
			continue;

		for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
			if (&i == &(*it) | (i.getType() == E_BULLET & i.getAncester() == (*it).getIdx()))
				continue;
			if (AABBCollision((*it).getBox(), i.getBox())) {
				std::cout << "충돌 일어남!" << std::endl;
				i.setDelete();
                eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),10,E_PACKET_HIT }, E_SEND);
                if ((*it).getHp() == 0 && x == 0)
                    x--;    // 한번만 보내라고 해놓은거 나중에 삭제
                    eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(), NULL, E_PACKET_DIE }, E_SEND);
				break;
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
	garbageTime -= deltaTime;
	if (garbageTime < FLT_EPSILON) {
		garbageColliection();
		garbageTime = 10.0f;
	}
}

int ObjectManager::addObject(value pos, value dir, value v, E_OBJECT_TYPE e) {
	objects.emplace_back(pos, dir, v, e);
	objects.back().setIdx(objects.size() - 1);
	return objects.size() - 1;
}

void ObjectManager::garbageColliection() {
	int count = 1;

	for (auto it = objects.begin()+3; it != objects.end(); ++it) {
		if (it->getDelete()) {
			objectSwap(*it, *(objects.end() - count));
			++count;
		}
	}
	if (count > 1) {
		auto it = objects.end() - (count - 1);
		objects.erase(it, objects.end());
		std::cout << "오브젝트 싹 지웠음 남은 오브젝트 갯수 :" << objects.size() << std::endl;
	}
}