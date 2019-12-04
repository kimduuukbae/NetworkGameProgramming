#include "ObjectManager.h"
#include "Structure.h"
#include <iostream>
#include "ServerDevice.h"
#include "EventManager.h"

ObjectManager::ObjectManager() : 
ItemCreateTime{ 5.0f },
windChangeTime{ 5.0f },
garbageTime {10.0f},
eventManager{ EventManager::instance() }
{}

//0 1 2 배, 3 바람 4 ~ 총알 아이템
void ObjectManager::update(double deltaTime){
	for (auto& i : objects) {
		if ((i.getType() == E_WIND) | i.getDelete() | !i.getLive())
			continue;

		for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
			if (!(*it).getLive() | (&i == &(*it)) | ((i.getType() == E_BULLET) & (i.getAncester() == (*it).getIdx())))
				continue;
			if (AABBCollision((*it).getBox(), i.getBox())) {
				std::cout << "충돌 일어남!" << std::endl;		
				i.setDelete();
				if (i.getType() == E_BULLET) {
					eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),findObject(i.getAncester()).getDamage(),E_PACKET_HIT }, E_SEND);
					(*it).manageHp(findObject(i.getAncester()).getDamage());
				}
				else if (i.getType() == E_ITEM)
				{
					short effect = rand() % 3;
					eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),(float)effect,E_PACKET_GETITEM }, E_SEND);
					switch (effect) {
					case 0:
						(*it).setMaxSpeed((*it).getMaxSpeed() + 5.f);
						break;
					case 1:
						(*it).setDamage((*it).getDamage() + 5);
						break;
					case 2:
						(*it).manageHp(-20);
						break;
					}
				}
				if ((*it).getHp() == 0) {
					(*it).setLive(false);
					eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(), NULL, E_PACKET_DIE }, E_SEND);
				}
				break;
			}
		}
		i.update(deltaTime);
	}
	auto [wvx, wvy, wvz] = findObject(3).getVelocity();
	for (auto& i : objects) {
		if (i.getType() == E_SHIP || i.getType() == E_ITEM || i.getType() == E_BULLET) {
			auto [x, y, z] = i.getPos();
			i.setPos(value{ x + wvx * (float)deltaTime,y + wvy * (float)deltaTime,z + wvz * (float)deltaTime });
		}
	}
	ItemCreateTime -= deltaTime;
	if (ItemCreateTime < FLT_EPSILON)
	{
		ItemCreateTime = 10.f;
		short effect = rand() % 3;
		short posX = rand() % (775 - (-775) + 1) + (-775);
		short posY = rand() % (425 - (-425) + 1) + (-425);
		addObject(value{ (float)posX,(float)posY,0.f }, value{ 0.f,0.f,0.f }, value{ 50.f,50.f,0.f }, E_ITEM);
		eventManager->pushEvent(itemPacket{ effect,posX,posY }, E_SEND);
	}
	windChangeTime -= deltaTime;
	if (windChangeTime < FLT_EPSILON)
	{
		windChangeTime = 30.f;
		short wind = -1;
		short windVelX = 0;// rand() % (10 - (-10) + 1) + (-10);
		short windVelY = 0;// rand() % (10 - (-10) + 1) + (-10);
		findObject(3).setVelocity(windVelX, windVelY, 0.f);
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