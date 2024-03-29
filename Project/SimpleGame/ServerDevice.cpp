#include "stdafx.h"
#include "ServerDevice.h"
#include <thread>
#include "ObjectManager.h"
#include "ship.h"
#include "bullet.h"
#include "item.h"
#include "Reef.h"
#include "Wind.h"
#include <iostream>
#include <mutex>
#include <string>

ObjectManager* objects = nullptr;
std::mutex m;
ServerDevice::ServerDevice(){
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
}

void ServerDevice::initialize(){
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::cout << "아이피를 입력해주세요 : " << std::endl;
	std::string s;
	std::cin >> s;

	serverAddr.sin_addr.s_addr = inet_addr(s.c_str());
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_family = AF_INET;

	connect(connectSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	makeThread();
}

void ServerDevice::sendData(const std::variant<simplePacket, shootPacket, posPacket, allPacket>& packet){
	packetHead h;
	int packetType = setHeadPacket(packet, h);	
	send(connectSocket, (char*)&h, sizeof(h), 0);
	if (packetType == 0) {
		simplePacket pack = std::get<0>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
	else if(packetType == 1) {
		shootPacket pack = std::get<1>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
	else if (packetType == 2) {
		posPacket pack = std::get<2>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
	else if (packetType == 3){
		allPacket pack = std::get<3>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
}

int ServerDevice::getId(){
	return myId;
}

int ServerDevice::getLive(){
	return live;
}

void ServerDevice::makeThread(){
	std::thread{ &ServerDevice::recvData, this }.detach();
}

void ServerDevice::recvData(){

	if (objects == nullptr)
		objects = ObjectManager::instance(); // lazy initialize
	
	while (1) {
		packetHead h;
		int retval = recvn(connectSocket, (char*)&h, sizeof(packetHead), 0);
		if (retval == SOCKET_ERROR)
			return;
		m.lock();
		switch (h.packetType) {
			case E_PACKET_SPEED: {
				simplePacket sim = recvSimplePacket();
				objects->getObject<Ship>(sim.id)->addSpeed(sim.value);
				break;
			}
			case E_PACKET_DEGREE: {
				simplePacket sim = recvSimplePacket();
				objects->getObject<Ship>(sim.id)->rotation(sim.value);
				break;
			}
			case E_PACKET_SHOOT: {
				shootPacket sht = recvshootPacket();
				int idx = objects->addObject<Bullet>(value{ (float)sht.tarPosX,(float)sht.tarPosY,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
					value{ 20.0f,20.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/bullet.png");
				auto t = objects->getObject<Bullet>(idx);
				t->setShipIdx(sht.id);
				t->setType(E_BULLET);
				t->process(sht.mposX, sht.mPosY, sht.tarPosX, sht.tarPosY);
				break;
			}
			case E_PACKET_HIT: {
				simplePacket sim = recvSimplePacket();
				objects->getObject<Ship>(sim.id)->manageHp(sim.value);
				auto o = objects->getObject<Ship>(sim.id);
				break;
			}
			case E_PACKET_DIE: {
				simplePacket sim = recvSimplePacket();
				objects->getObject<Ship>(sim.id)->setLive(false);
				live -= 1;
				if (live == 1)
					objects->addObject<Object>(value{ 0.f,0.f,0.f }, color{ 1.f,1.f,1.f,1.f },
						value{ 800.0f,200.f,0.f }, value{ 0.f,0.f,0.f }, "texture/finale.png");
				break;
			}
			case E_PACKET_SENID: {
				simplePacket sim = recvSimplePacket();
				myId = h.id;	
				objects->getObject<Ship>(myId)->setPngIdx(objects->getPngIdx("texture/green_ship.png"));
				
				for (int i = 0; i < 3; i++) {
					if (myId != i) 
						objects->getObject<Ship>(i)->setPngIdx(objects->getPngIdx("texture/red_ship.png"));
				}
				break;
			}
			case E_PACKET_OTSET: {
				posPacket pos = recvposPacket();
				auto o = objects->getObject<Ship>(pos.id);
				o->setShipIdx(pos.id);
				o->setPos(pos.posX, pos.posY, 0);
				if ((pos.id == 2) &objects->getObject<Object>(4)->getType() != E_REEF) {
					objects->getObject<Object>(4)->setDelete();
					live = 3;
				}
				break;
			}
			case E_PACKET_SYNC: {
				allPacket all = recvallPacket();
				auto o = objects->getObject<Ship>(all.id);
				auto [x, y, z] = o->getPos();
				o->setPos(all.x, all.y, 0.0f);
				o->setVelocity(all.velx, all.vely, 0.0f);
				break;
			}
			case E_PACKET_ITEM: {
				itemPacket item = recvItemPacket();
				if (item.effect == 0) {			// 속도 업
					int idx = objects->addObject<SpeedItem>(value{ (float)item.itemPosX,(float)item.itemPosY,0.f }, color{ 1.0f,1.0f,1.0f,1.0f },
						value{ 50.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/item.png");
					auto o = objects->getObject(idx);
					o->setType(E_ITEM);
				}
				else if (item.effect == 1) {	// 공격력 업
					int idx = objects->addObject<DamageItem>(value{ (float)item.itemPosX,(float)item.itemPosY,0.f }, color{ 1.0f,1.0f,1.0f,1.0f },
						value{ 50.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/item.png");
					auto o = objects->getObject(idx);
					o->setType(E_ITEM);
				}
				else {							// 체력 업
					int idx = objects->addObject<HealItem>(value{ (float)item.itemPosX,(float)item.itemPosY,0.f }, color{ 1.0f,1.0f,1.0f,1.0f },
						value{ 50.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/item.png");
					auto o = objects->getObject(idx);
					o->setType(E_ITEM);
				}
				break;
			}
			case E_PACKET_WIND: {
				itemPacket wind = recvItemPacket();
				for (Object* o : objects->getObjects()) {
					if (o->getType() == E_WIND) {
						o->setVelocity(Vector3D{ (float)wind.itemPosX,(float)wind.itemPosY,0.f });
						auto [x, y, z] = o->getVelocity();
						float degree = atan2(y, x) * 180 / 3.14;
						o->setDegree(degree);
						break;
					}
				}
				break;
			}
			case E_PACKET_REEF: {
				itemPacket reef = recvItemPacket();
				int idx = objects->addObject<Reef>(value{ (float)reef.itemPosX,(float)reef.itemPosY,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
					value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/reef.png");
				auto t = objects->getObject(idx);
				t->setType(E_REEF);
				break;
			}
			case E_PACKET_GETITEM: {
				simplePacket sim = recvSimplePacket();
				auto o = objects->getObject<Ship>(sim.id);
					switch ((short)sim.value) {
					case 0: {
						float curMaxSpeed = o->getMaxSpeed();
						o->setMaxSpeed(curMaxSpeed + 5.f);
						break;
					}
					case 1: {
						float curBulletDamage = o->getDamage();
						o->setDamage(curBulletDamage + 5);
						break;
					}
					case 2: {
						o->manageHp(-20);
						if (o->getHp() > 100)
							o->setHp(100);
						break;
					}
				}
				printf("%d번 배 - 최대속력: %f 공격력: %d 체력: %d\n", sim.id, o->getMaxSpeed(), o->getDamage(), o->getHp());
				break;
			}
			case E_PACKET_RESET: {
				simplePacket sim = recvSimplePacket();
				auto o = objects->getObjects();
				for (auto it = o.begin() + 8; it != o.end(); ++it)
					(*it)->setDelete();

				objects->garbageCollection();
				for (int i = 0; i < 3; ++i) {
					auto ship = objects->getObject<Ship>(i);
					ship->setHp(100);
					ship->setLive(true);
					ship->setDirection(value{ 1.0f,0.0f,0.0f });
					ship->setVelocity(0.0f, 0.0f, 0.0f);
					ship->setRadian(0.0f);
					ship->setDamage(10);
					ship->setMaxSpeed(35.f);
				}
				auto wind = objects->getObject<Wind>(3);
				wind->setVelocity(0.f, 0.f, 0.f);
				wind->setDegree(0.f);
				live = 3;
				break;
			}
		}
		m.unlock();
	}
}

int ServerDevice::recvn(SOCKET s, char * buf, int len, int flags){
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

simplePacket ServerDevice::recvSimplePacket(){
	simplePacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

shootPacket ServerDevice::recvshootPacket(){
	shootPacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

posPacket ServerDevice::recvposPacket(){
	posPacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

allPacket ServerDevice::recvallPacket(){
	allPacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

itemPacket ServerDevice::recvItemPacket()
{
	itemPacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

int ServerDevice::setHeadPacket(const std::variant<simplePacket, shootPacket, posPacket, allPacket>& packet, packetHead& h){
	int count = -1;
	if (auto pack = std::get_if<0>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = pack->packetType;
		count = 0;
	}
	else if (auto pack = std::get_if<1>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = E_PACKET_SHOOT;
		count = 1;
	}
	else if (auto pack = std::get_if<2>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = E_PACKET_OTSET;
		count = 2;
	}
	else if (auto pack = std::get_if<3>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = E_PACKET_SYNC;
		count = 3;
	}
	return count;	//RVO..?
}
