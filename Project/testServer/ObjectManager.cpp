#include "ObjectManager.h"
#include "Structure.h"
#include "ServerDevice.h"
#include "EventManager.h"
#include "Event.h"


ObjectManager::ObjectManager() :
	ItemCreateTime{ 20.0f },
	windChangeTime{ 20.0f },
	garbageTime{ 10.0f },
	live{ 3 },
	rTime{ 0.0f },
	eventManager{ EventManager::instance() }
{}

void ObjectManager::update(float deltaTime) {
	if (live != 1) {
		for (auto& i : objects) {
			if ((i.getType() == E_WIND) | i.getDelete() | !i.getLive())
				continue;

			for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
				if (!(*it).getLive() | (&i == &(*it)) | ((i.getType() == E_BULLET) & (i.getAncester() == (*it).getIdx())))
					continue;
				if (AABBCollision((*it).getBox(), i.getBox())) {
					if (i.getType() == E_BULLET) {
						collideBulletToShip(i, it);
						i.setDelete();
					}
					else if (i.getType() == E_ITEM) {
						collideItemToShip(i, it);
						i.setDelete();
					}
					else if (i.getType() == E_REEF)
						collideReefToShip(i, it);

					else if (i.getType() == E_SHIP)
						collideShipToShip(i, it);
					break;
				}
			}

			for (auto it = objects.begin() + 4; it != objects.begin() + 7; ++it) {
				if (AABBCollision((*it).getBox(), i.getBox())) {
					if (i.getType() == E_BULLET)
						i.setDelete();
					else if (i.getType() == E_ITEM)
						i.setCollobject(true);
				}
			}
			i.update(deltaTime);
		}
		auto[wvx, wvy, wvz] = findObject(3).getVelocity();
		for (auto& i : objects) {
			if ((i.getType() == E_SHIP) | (i.getType() == E_ITEM) | (i.getType() == E_BULLET)) {
				if (!i.getCollobject()) {
					auto[x, y, z] = i.getPos();
					i.setPos(value{ x + wvx * (float)deltaTime,y + wvy * (float)deltaTime,z + wvz * (float)deltaTime });
				}
			}
		}
		ItemCreateTime -= deltaTime;
		if (ItemCreateTime < FLT_EPSILON)
		{
			ItemCreateTime = 20.f;
			short effect = rand() % 3;
			short posX = rand() % (775 - (-775) + 1) + (-775);
			short posY = rand() % (425 - (-425) + 1) + (-425);
			addObject(value{ (float)posX,(float)posY,0.f }, value{ 0.f,0.f,0.f }, value{ 50.f,50.f,0.f }, E_ITEM);
			eventManager->pushEvent(itemPacket{ effect,posX,posY }, E_SEND);
		}
		windChangeTime -= deltaTime;
		if (windChangeTime < FLT_EPSILON)
		{
			windChangeTime = 20.f;
			short wind = -1;
			short windVelX = rand() % (10 - (-10) + 1) + (-10);
			short windVelY = rand() % (10 - (-10) + 1) + (-10);
			findObject(3).setVelocity(windVelX, windVelY, 0.f);
			eventManager->pushEvent(itemPacket{ wind,windVelX,windVelY }, E_SEND);
		}
		garbageTime -= deltaTime;
		if (garbageTime < FLT_EPSILON)
			garbageColliection();
	}
	else {
		rTime += deltaTime;
		if (rTime > 5.f) {
			ItemCreateTime = 20.f;
			windChangeTime = 20.f;
			findObject(3).setVelocity(0.f, 0.f, 0.f);
			for (auto it = objects.begin(); it != objects.begin() + 3; ++it) {
				value v[3] = { {-400.f,-200.f,0.f},{-400.f,300.f,0.f},{400.f,-100.f,0.f} };
				(*it).setPos(v[(*it).getIdx()]);
				(*it).setLive(true);
				(*it).manageHp(-100);
				(*it).setVelocity(0.f, 0.f, 0.f);
				(*it).setDirection(1.f, 0.f, 0.f);
				(*it).setDamage(10);
				(*it).setMaxSpeed(35.f);
			}
			live = 3;
			rTime = 0.f;
			eventManager->pushEvent(posPacket{ 0, -400,-200 }, E_SEND);
			eventManager->pushEvent(posPacket{ 1, -400,300 }, E_SEND);
			eventManager->pushEvent(posPacket{ 2, 400,-100 }, E_SEND);
			eventManager->pushEvent(simplePacket{ (char)(0), 0, E_PACKET_RESET }, E_SEND);
			for (auto it = objects.begin() + 8; it != objects.end(); ++it)
				it->setDelete();
			garbageColliection();
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
	garbageTime = 10.0f;
}

void ObjectManager::collideBulletToShip(Object& o, const std::vector<Object>::iterator& shp) {
	eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),(float)findObject(o.getAncester()).getDamage(),E_PACKET_HIT }, E_SEND);
	(*shp).manageHp(findObject(o.getAncester()).getDamage());
	if ((*shp).getHp() < 1) {
		(*shp).setLive(false);
		eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(), 0, E_PACKET_DIE }, E_SEND);
		live--;
	}
}

void ObjectManager::collideItemToShip(Object & o, const std::vector<Object>::iterator & shp) {
	short effect = rand() % 3;
	eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),(float)effect,E_PACKET_GETITEM }, E_SEND);
	if (effect == 0)
		(*shp).setMaxSpeed((*shp).getMaxSpeed() + 5.f);
	else if (effect == 1)
		(*shp).setDamage((*shp).getDamage() + 5);
	else if (effect == 2)
		(*shp).manageHp(-20);
}

void ObjectManager::collideReefToShip(Object & o, const std::vector<Object>::iterator & shp) {
	value dir = (*shp).getDirection();
	value rpos = o.getPos() - (*shp).getPos();
	float degree = fabsf(atan2(dir.y, dir.x) * 180 / 3.14 - atan2(rpos.y, rpos.x) * 180 / 3.14);
	if (degree <= 45)
		(*shp).setVelocity(0.f, 0.f, 0.f);
}

void ObjectManager::collideShipToShip(Object & o, const std::vector<Object>::iterator & shp) {
	value fshipdir = o.getDirection();
	value sshipdir = (*shp).getDirection();
	float fshipdirdegree = radToDegree(atan2(fshipdir.y, fshipdir.x));
	float sshipdirdegree = radToDegree(atan2(sshipdir.y, sshipdir.x));
	float degree = fabs(fshipdirdegree - sshipdirdegree);

	Vector3D v1 = o.getVelocity();
	Vector3D v2 = (*shp).getVelocity();

	if ((v1.size() < 2.0f) | (v2.size() < 2.0f))
		return;

	if (degree < 90) {
		if (fshipdir.y >= 0 && sshipdir.y >= 0) {
			if (o.getPos().y > (*shp).getPos().y) {
				o.manageHp(10);
				(*shp).manageHp(5);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),10,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),5,E_PACKET_HIT }, E_SEND);
			}
			else {
				o.manageHp(5);
				(*shp).manageHp(10);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),5,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),10,E_PACKET_HIT }, E_SEND);
			}
		}
		else if (fshipdir.y < 0 && sshipdir.y < 0) {
			if (o.getPos().y < (*shp).getPos().y) {
				o.manageHp(5);
				(*shp).manageHp(10);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),5,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),10,E_PACKET_HIT }, E_SEND);
			}
			else {
				o.manageHp(10);
				(*shp).manageHp(5);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),10,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),5,E_PACKET_HIT }, E_SEND);
			}
		}
		else if (fshipdir.x >= 0 && sshipdir.x >= 0) {
			if (o.getPos().x > (*shp).getPos().x) {
				o.manageHp(10);
				(*shp).manageHp(5);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),10,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),5,E_PACKET_HIT }, E_SEND);
			}
			else {
				o.manageHp(5);
				(*shp).manageHp(10);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),5,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),10,E_PACKET_HIT }, E_SEND);
			}
		}
		else if (fshipdir.x < 0 && sshipdir.x < 0) {
			if (o.getPos().x < (*shp).getPos().x) {
				o.manageHp(5);
				(*shp).manageHp(10);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),5,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),10,E_PACKET_HIT }, E_SEND);
			}
			else {
				o.manageHp(10);
				(*shp).manageHp(5);
				eventManager->pushEvent(simplePacket{ (char)o.getIdx(),10,E_PACKET_HIT }, E_SEND);
				eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),5,E_PACKET_HIT }, E_SEND);
			}
		}
	}
	else {
		o.manageHp(10);
		(*shp).manageHp(10);
		eventManager->pushEvent(simplePacket{ (char)o.getIdx(),10,E_PACKET_HIT }, E_SEND);
		eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(),10,E_PACKET_HIT }, E_SEND);
	}

	o.setVelocity(0.f, 0.f, 0.f);
	(*shp).setVelocity(0.f, 0.f, 0.f);
	if (o.getHp() < 1) {
		o.setLive(false);
		eventManager->pushEvent(simplePacket{ (char)o.getIdx(), 0, E_PACKET_DIE }, E_SEND);
		live--;
	}
	if ((*shp).getHp() < 1) {
		(*shp).setLive(false);
		eventManager->pushEvent(simplePacket{ (char)(*shp).getIdx(), 0, E_PACKET_DIE }, E_SEND);
		live--;
	}
}
