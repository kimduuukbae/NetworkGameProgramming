#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "ship.h"
#include "testObject.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include <iostream>
#include "Input.h"

void MenuScene::init(){
	o = D_OBJECT;

	o->addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	o->addObject<test>(value{ 200.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	o->addObject<test>(value{ -200.0f,0.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	o->addObject<test>(value{ 0.0f,200.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	o->addObject<test>(value{ 0.0f,-200.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	v = o->getObjects();

	v[0]->setType(E_SHIP);
	v[1]->setType(E_ITEM);
	v[2]->setType(E_ITEM);
	v[3]->setType(E_ITEM);
	v[4]->setType(E_ITEM);
}

void MenuScene::update(float dt){
	o->update(dt);
	if (auto ship = o->getObject<Ship>(0); ship != nullptr) {
		if (D_INPUT->isKeyDown(VK_UP)) 
			ship->changePushType(E_PUSH);
		
		if (D_INPUT->isKeyUp(VK_UP)) 
			ship->changePushType(E_RELEASED);
	}
}

void MenuScene::draw(){
	o->draw();
}

void MenuScene::destroy(){
	o->destroy();
}
