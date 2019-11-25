#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "ship.h"
#include "bullet.h"
#include "ObjectManager.h"
#include "Input.h"
#include "Item.h"
#include "Reef.h"
#include "Wind.h"
#include <limits>


void MenuScene::init(){	
	o = D_OBJECT;
	windChangeCoolTime = defaultWindCoolTime;
	itemCreationCoolTime = defaultItemCoolTime;

	o->addObject<Ship>(value{ -1000.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
	o->addObject<Ship>(value{ -1000.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
	o->addObject<Ship>(value{ -1000.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");

	o->addObject<Reef>(value{ 0.0f,200.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 80.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/speed.png");

	o->addObject<Wind>(value{ -800.0f,-350.0f,0.0f }, color{ 0.0f,0.0f,0.0f,1.0f },
		value{ 100,100,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/speed.png");

	v = o->getObjects();

	v[0]->setType(E_SHIP);
	v[1]->setType(E_SHIP);
	v[2]->setType(E_SHIP);

	v[3]->setType(E_REEF);
	v[4]->setType(E_WIND);

	serverDevice.initialize();
}

void MenuScene::update(float dt){
	o->update(dt);
	shootDelay += dt;
	if (auto ship = o->getObject<Ship>(serverDevice.getId()); ship != nullptr) {
		if (D_INPUT->isKeyOverlap(VK_UP))
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), 1.0f, E_PACKET_SPEED });
		
		if (D_INPUT->isKeyUp(VK_UP)) 
			ship->changePushType(E_RELEASED);

		if (D_INPUT->isKeyOverlap(VK_LEFT)) 
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), 0.01f, E_PACKET_DEGREE });
		

		if (D_INPUT->isKeyOverlap(VK_RIGHT))
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), -0.01f, E_PACKET_DEGREE });
		
		if (ship->getPushType() == E_RELEASED)
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), -1.0f, E_PACKET_SPEED });

		count = ship->getbulletCooltime();

		if (D_INPUT->isMouseDown() && shootDelay > 0.3f && ship->getbulletCooltime() > 0) {
			ship->setbulletCooltime(--count);
			shootDelay = 0.0f;

			value v = ship->getPos();

			serverDevice.sendData(shootPacket{ (char)serverDevice.getId(), (short)D_INPUT->mx, (short)D_INPUT->my, (short)v.x, (short)v.y });
			printf("%d %d %f %f\n", (short)v.x, (short)v.y, v.x, v.y);
		}
	}
	windChangeCoolTime -= dt;
	if (windChangeCoolTime < std::numeric_limits<float>::epsilon()) {
		v = o->getObjects();
		for (auto& obj : v) {
			if (obj->getType() == E_WIND) {
				auto wind = obj->getObjectCast<Wind>(obj);
				wind->setWind(Vector3D(0.f, 0.f, 0.f));
				windChangeCoolTime = defaultWindCoolTime;
				break;
			}
		}
	}
	itemCreationCoolTime -= dt;
	if (itemCreationCoolTime < std::numeric_limits<float>::epsilon()) {
		// ·£´ýÇÑ À§Ä¡¿¡ ¾ÆÀÌÅÛÀ» ÄðÅ¸ÀÓ¸¶´Ù »ý¼º ·£´ý ÁÂÇ¥´Â ±¸Çö ¾ÈµÊ
		//o->addObject<Item>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
			//value{ 50.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
		itemCreationCoolTime = defaultItemCoolTime;
	}
}

void MenuScene::draw(){
	o->draw();
}

void MenuScene::destroy(){
	o->destroy();
}
