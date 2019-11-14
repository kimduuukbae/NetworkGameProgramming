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
	//	0��° ������Ʈ�� �浹�� ��� �ֵ��� ������
	auto list = o->getCollisionObjectList(o->getObject(0));
	if (list != nullptr) {
		for (auto& i : *list) {
			int type = i->getType();	// �̷��� Ÿ���� ���� �� ����
			if (type == E_ITEM)
				std::cout << "�浹�ȳ��� ������Ÿ����" << std::endl;
		}
	}
}

void MenuScene::draw(){
	o->draw();
}

void MenuScene::destroy(){
	
}
