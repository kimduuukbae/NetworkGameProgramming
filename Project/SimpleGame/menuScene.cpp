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
#include "PhysicsComponent.h"
#include <limits>


void MenuScene::init(){	
	o = D_OBJECT;
	o->preGenerateImage("texture/ship.png");
	o->preGenerateImage("texture/item.png");
	o->preGenerateImage("texture/bullet.png");
	o->preGenerateImage("texture/backWind.png");
	o->preGenerateImage("texture/waiting.png");
	o->preGenerateImage("texture/reef.png");
	o->preGenerateImage("texture/reef_2.png");
	o->preGenerateImage("texture/reef_3.png");
	o->preGenerateImage("texture/wind.png");
	o->preGenerateImage("texture/green_ship.png");
	o->preGenerateImage("texture/red_ship.png");

	o->addObject<Ship>(value{ -1000.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
	o->addObject<Ship>(value{ -1000.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
	o->addObject<Ship>(value{ -1000.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/ship.png");
	o->addObject<Wind>(value{ -700.f,-350.f,0.f }, color{ 0.f,0.f,0.f,0.f },
		value{ 150.f,150.f,0.f }, value{ 0.f,0.f,0.f }, "texture/wind.png");
	o->addObject<Object>(value{ 0.f,0.f,0.f }, color{ 1.f,1.f,1.f,1.f },
		value{ 800.0f,200.f,0.f }, value{ 0.f,0.f,0.f }, "texture/waiting.png");
	
	v = o->getObjects();

	v[0]->setType(E_SHIP);
	v[1]->setType(E_SHIP);
	v[2]->setType(E_SHIP);
	v[3]->setType(E_WIND);

	serverDevice.initialize();
}

void MenuScene::update(float dt){
	o->update(dt);
	if (o->getObject<Wind>(3)->getVelocity().size() > 0.1f) {
		auto[x, y, z] = o->getObject<Wind>(3)->getVelocity();
		for (auto& i : o->getObjects()) {
			if (i->getComponent<IPhysicsComponent>() != nullptr) {
				auto[sx, sy, sz] = i->getPos();
				i->setPos(sx + x * dt, sy + y * dt, sz + z * dt);
			}
		}
	}

	shootDelay += dt;
	if (auto ship = o->getObject<Ship>(serverDevice.getId()); ship != nullptr) {
		if (D_INPUT->isKeyDown(VK_UP))
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), 1.0f, E_PACKET_SPEED });
		
		if (D_INPUT->isKeyUp(VK_UP)) 
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), -1.0f, E_PACKET_SPEED });

		if (D_INPUT->isKeyOverlap(VK_LEFT)) 
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), 0.01f, E_PACKET_DEGREE });
		

		if (D_INPUT->isKeyOverlap(VK_RIGHT))
			serverDevice.sendData(simplePacket{ (char)serverDevice.getId(), -0.01f, E_PACKET_DEGREE });

		count = ship->getbulletCooltime();

		if (D_INPUT->isMouseDown() && shootDelay > 0.3f && ship->getbulletCooltime() > 0) {
			ship->setbulletCooltime(--count);
			shootDelay = 0.0f;

			value v = ship->getPos();
			serverDevice.sendData(shootPacket{ (char)serverDevice.getId(), (short)D_INPUT->mx, (short)D_INPUT->my, (short)v.x, (short)v.y });
		}
	}	
}

void MenuScene::draw(){
	o->draw();
	
}

void MenuScene::destroy(){
	o->destroy();
}
