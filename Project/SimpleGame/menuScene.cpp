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

	o->addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
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
	if (auto ship = o->getObject<Ship>(0); ship != nullptr) {
		if (D_INPUT->isKeyDown(VK_UP)) 
			ship->changePushType(E_PUSH);
		
		if (D_INPUT->isKeyUp(VK_UP)) 
			ship->changePushType(E_RELEASED);

		if (D_INPUT->isKeyOverlap(VK_LEFT))
			ship->leftRotation();

		if (D_INPUT->isKeyOverlap(VK_RIGHT))
			ship->rightRotation();
		
		count = ship->getbulletCooltime();

		if (D_INPUT->isMouseDown() && shootDelay > 1.f && ship->getbulletCooltime() > 0) {
			ship->setbulletCooltime(--count);
			int idx = o->addObject<Bullet>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
				value{ 20.0f,20.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/bullet.png");

			auto t = o->getObject<Bullet>(idx);
			t->setShipIdx(0);
			t->setType(E_BULLET);
			t->process(o->getObject<Ship>(0), dt);

			shootDelay = 0.0f;
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
		// ������ ��ġ�� �������� ��Ÿ�Ӹ��� ���� ���� ��ǥ�� ���� �ȵ�
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
