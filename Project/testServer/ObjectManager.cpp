#include "ObjectManager.h"
#include "Structure.h"
#include <iostream>
#include "ServerDevice.h"
#include "EventManager.h"
#include "Event.h"


ObjectManager::ObjectManager() :
	ItemCreateTime{ 5.0f },
	windChangeTime{ 5.0f },
	garbageTime{ 10.0f },
	eventManager{ EventManager::instance() }
{}

//0 1 2 배, 3 바람 4~6 암초 7 ~ 총알 아이템
void ObjectManager::update(double deltaTime) {
	if (live != 1) {
		for (auto& i : objects) {
			if ((i.getType() == E_WIND) | i.getDelete() | !i.getLive())
				continue;

			for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
				if (!(*it).getLive() | (&i == &(*it)) | ((i.getType() == E_BULLET) & (i.getAncester() == (*it).getIdx())))
					continue;
				if (AABBCollision((*it).getBox(), i.getBox())) {
					if (i.getType() == E_BULLET) {
						eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),(float)findObject(i.getAncester()).getDamage(),E_PACKET_HIT }, E_SEND);
						(*it).manageHp(findObject(i.getAncester()).getDamage());
						if ((*it).getHp() < 1) {
							(*it).setLive(false);
							eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(), 0, E_PACKET_DIE }, E_SEND);
							live--;
						}
						i.setDelete();
					}
					i.setDelete();
				}
				else if (i.getType() == E_ITEM) {
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
					i.setDelete();
				}
				else if (i.getType() == E_REEF) {
					value dir = (*it).getDirection();
					value rpos = i.getPos() - (*it).getPos();
					float degree = fabs(atan2(dir.y, dir.x) * 180 / 3.14 - atan2(rpos.y, rpos.x) * 180 / 3.14);
					if (degree <= 45)
						(*it).setVelocity(0.f, 0.f, 0.f);
				}
				else if (i.getType() == E_SHIP) {
					value fshipdir = i.getDirection();
					value sshipdir = (*it).getDirection();
					float fshipdirdegree = radToDegree(atan2(fshipdir.y, fshipdir.x));
					float sshipdirdegree = radToDegree(atan2(sshipdir.y, sshipdir.x));
					float degree = fabs(fshipdirdegree - sshipdirdegree);
					if (degree < 90) {
						if (fshipdir.y >= 0 && sshipdir.y >= 0) {
							if (i.getPos().y > (*it).getPos().y) {
								i.manageHp(10);
								(*it).manageHp(5);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),10,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),5,E_PACKET_HIT }, E_SEND);
							}
							else {
								i.manageHp(5);
								(*it).manageHp(10);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),5,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),10,E_PACKET_HIT }, E_SEND);
							}
						}
						else if (fshipdir.y < 0 && sshipdir.y < 0) {
							if (i.getPos().y < (*it).getPos().y) {
								i.manageHp(5);
								(*it).manageHp(10);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),5,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),10,E_PACKET_HIT }, E_SEND);
							}
							else {
								i.manageHp(10);
								(*it).manageHp(5);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),10,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),5,E_PACKET_HIT }, E_SEND);
							}
						}
						else if (fshipdir.x >= 0 && sshipdir.x >= 0) {
							if (i.getPos().x > (*it).getPos().x) {
								i.manageHp(10);
								(*it).manageHp(5);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),10,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),5,E_PACKET_HIT }, E_SEND);
							}
							else {
								i.manageHp(5);
								(*it).manageHp(10);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),5,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),10,E_PACKET_HIT }, E_SEND);
							}
						}
						else if (fshipdir.x < 0 && sshipdir.x < 0) {
							if (i.getPos().x < (*it).getPos().x) {
								i.manageHp(5);
								(*it).manageHp(10);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),5,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),10,E_PACKET_HIT }, E_SEND);
							}
							else {
								i.manageHp(10);
								(*it).manageHp(5);
								eventManager->pushEvent(simplePacket{ (char)i.getIdx(),10,E_PACKET_HIT }, E_SEND);
								eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(),5,E_PACKET_HIT }, E_SEND);
							}
						}
					}
					else {
						i.manageHp(10);
						(*it).manageHp(10);
					}
					i.setVelocity(0.f, 0.f, 0.f);
					(*it).setVelocity(0.f, 0.f, 0.f);
				}
			}

			// 암초 충돌 체크
			for (auto it = objects.begin() + 4; it != objects.begin() + 7; ++it) {
				if (AABBCollision((*it).getBox(), i.getBox())) {
					if (i.getType() == E_BULLET)
						i.setDelete();
					else if (i.getType() == E_ITEM) {
						i.setCollobject(true);
					}
				}
				i.update(deltaTime);
			}
			i.update(deltaTime);
		}
		auto [wvx, wvy, wvz] = findObject(3).getVelocity();
		for (auto& i : objects) {
			if (i.getType() == E_SHIP | i.getType() == E_ITEM | i.getType() == E_BULLET) {
				if (!i.getCollobject()) {
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
				windChangeTime = 5.f;
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
	}
	else {
		rTime += deltaTime;
		if (rTime > 5.f) {
			std::cout << "reset start" << std::endl;
			for (auto it = objects.begin() + 7; it <= objects.end(); ++it) {
				(*it).setDelete();
				eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(), 0, E_PACKET_RESET }, E_SEND);
			}
			for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
				value v[3] = { {-400.f,-200.f,0.f},{-400.f,300.f,0.f},{400.f,-100.f,0.f} };
				(*it).setPos(v[(*it).getIdx()]);
				(*it).setLive(true);
				(*it).manageHp(-100);
				(*it).setVelocity(0.f, 0.f, 0.f);
				(*it).setDirection(1.f, 0.f, 0.f);
				posPacket p1{ 0, -400,-200 };
				posPacket p2{ 1, -400,300 };
				posPacket p3{ 2, 400,-100 };

				eventManager->pushEvent(p1, E_SEND);
				eventManager->pushEvent(p2, E_SEND);
				eventManager->pushEvent(p3, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*it).getIdx(), 0, E_PACKET_RESET }, E_SEND);
			}
			std::cout << "reset end" << std::endl;
			live = 3;
			rTime = 0.f;
		}
	}
	}

	int ObjectManager::addObject(value pos, value dir, value v, E_OBJECT_TYPE e) {
		objects.emplace_back(pos, dir, v, e);
		objects.back().setIdx(objects.size() - 1);
		return objects.size() - 1;
	}

	void ObjectManager::garbageColliection() {
		int count = 1;

		for (auto it = objects.begin() + 3; it != objects.end(); ++it) {
			if (it->getDelete()) {
				objectSwap(*it, *(objects.end() - count));
				++count;
			}
		}
		if (count > 1) {
			auto it = objects.end() - (count - 1);
			objects.erase(it, objects.end());
		}
	}