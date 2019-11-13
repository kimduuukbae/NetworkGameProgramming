#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "ship.h"
#include "testObject.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include <iostream>
void MenuScene::init(){
	o = D_OBJECT;
	o->addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 1.0f,1.0f,1.0f,1.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
<<<<<<< HEAD
	o->addObject<test>(value{ 200.0f,200.0f,0.0f }, color{ 0.0f,0.0f,0.0f,0.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	v = o->getObjects();
	garbagetime = 0.0f;
	v[0]->setType(E_SHIP);
	v[1]->setType(E_ITEM);	
	// 첫번째로 만들어진놈은 ship 타입, 두번째는 item타입임!
=======
	v = o->getObject();
	for (auto it = v->begin() ; it != v->end(); ++it) {
		(*it)->setDelete();
	}
>>>>>>> lyj
}

void MenuScene::update(float dt){
	o->update(dt);
<<<<<<< HEAD
	garbagetime += dt;
	//	0번째 오브젝트와 충돌된 모든 애들을 가져옴
	auto list = o->getCollisionObjectList(o->getObject(0));
	if (list != nullptr) {
		for (auto& i : *list) {
			int type = i->getType();	// 이렇게 타입을 꺼낼 수 있음
			if (type == E_ITEM)
				std::cout << "충돌된놈은 아이템타입임" << std::endl;
		}
	}
	if (garbagetime > 5.0f) {
		o->garbageCollection();
		garbagetime = 0.0f;
	}
=======
	o->garbageCollection();
>>>>>>> lyj
}

void MenuScene::draw(){
	o->draw();
}

void MenuScene::destroy(){
	
}
