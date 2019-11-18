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
#include <limits>

void MenuScene::init(){
	o = D_OBJECT;

	windChangeCoolTime = defaultWindCoolTime;
	itemCreationCoolTime = defaultItemCoolTime;

	o->addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");

	o->addObject<Reef>(value{ 0.0f,200.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 80.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/speed.png");

	o->addObject<Wind>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,1.0f },
		value{ 100,100,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/speed.png");

	v = o->getObjects();

	v[0]->setType(E_SHIP);
	Object::getObjectCast<Ship>(v[0])->setShipIdx(0);
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

		if (D_INPUT->isKeyOverlap(VK_LEFT))
			ship->leftRotation();

		if (D_INPUT->isKeyOverlap(VK_RIGHT))
			ship->rightRotation();
		
		if (D_INPUT->isMouseDown()) {
			int idx = o->addObject<Bullet>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
				value{ 20.0f,20.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/bullet.png");
			
			auto t = o->getObject<Bullet>(idx);
			t->setShipIdx(0);
			t->setType(E_BULLET);
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
