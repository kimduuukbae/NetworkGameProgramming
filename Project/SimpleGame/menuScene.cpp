#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "ship.h"
#include "bullet.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include "Input.h"
#include "Item.h"
#include "Reef.h"
#include "Wind.h"

void MenuScene::init(){
	o = D_OBJECT;

	windChangeCoolTime = defaultWindCoolTime;
	itemCreationCoolTime = defaultItemCoolTime;

	o->addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");

	o->addObject<Reef>(value{ 0.0f,200.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 80.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/speed.png");

	o->addObject<Wind>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,1.0f },
		value{ 1600.0f,900.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/speed.png");

	v = o->getObjects();

	v[0]->setType(E_SHIP);
	v[0]->getObjectCast<Ship>(v[0])->setShipIdx(0);
	v[1]->setType(E_REEF);
	v[2]->setType(E_WIND);
}

void MenuScene::update(float dt){
	o->update(dt);
	if (auto ship = o->getObject<Ship>(0); ship != nullptr) {
		if (D_INPUT->isKeyDown(VK_UP)) 
			ship->changePushType(E_PUSH);
		
		if (D_INPUT->isKeyUp(VK_UP)) 
			ship->changePushType(E_RELEASED);

		if (D_INPUT->isMouseDown()) {
			int idx = o->addObject<Bullet>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
				value{ 20.0f,20.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/bullet.png");
			
			auto t = o->getObject<Bullet>(idx);
			t->setShipIdx(0);
			t->setType(E_BULLET);
		}
	}
	windChangeCoolTime -= dt;
	if (windChangeCoolTime < 0.0000000001f) {
		v = o->getObjects();
		for (auto& obj : v) {
			if (obj->getType() == E_WIND) {
				auto wind = obj->getObjectCast<Wind>(obj);
				wind->setWind(Vector3D(0.f, 0.f, 0.f));
				break;
			}
		}
	}
	itemCreationCoolTime -= dt;
	if (itemCreationCoolTime < 0.0000000001f) {
		// ������ ��ġ�� �������� ��Ÿ�Ӹ��� ���� ���� ��ǥ�� ���� �ȵ�
		o->addObject<Item>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
			value{ 50.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
	}
}

void MenuScene::draw(){
	o->draw();
}

void MenuScene::destroy(){
	o->destroy();
}
