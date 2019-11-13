#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "Define.h"
#include "ship.h"
#include "ObjectManager.h"
void MenuScene::init(){
	o = D_OBJECT;
	//lists.push_back(addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 1.0f,1.0f,1.0f,1.0f },
	//	value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, renderer->GenPngTexture("texture/title.png")));
	o->addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 1.0f,1.0f,1.0f,1.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, "texture/title.png");
	v = o->getObject();
	for (auto it = v->begin() ; it != v->end(); ++it) {
		(*it)->setDelete();
	}
}

void MenuScene::update(float dt){
	o->update(dt);
	o->garbageCollection();
}

void MenuScene::draw(){
	o->draw();
}

void MenuScene::destroy(){
	delete renderer;
}
