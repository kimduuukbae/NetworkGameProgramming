#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "Define.h"
#include "Object.h"
#include "testObject.h"
#include "Renderer.h"

void MenuScene::init(){
	renderer = new Renderer(500, 500);
	if (!renderer->IsInitialized())
		return;
	lists.push_back(new testObject());
	lists.back()->setPos(0, 0, 0);
	lists.back()->setVolume(100, 100, 0);
	lists.back()->setMass(0.5f);
	lists.back()->setVelocity(0.0f, 0.0f, 0.0f);
	lists.back()->setIdx(renderer->GenPngTexture("texture/title.png"));
}

void MenuScene::update(float dt){
	for (auto& i : lists)
		i->update(dt);
}

void MenuScene::draw(){
	for (auto& i : lists) {
		auto[x, y, z] = i->getPos();
		auto[sx, sy, sz] = i->getVolume();
		renderer->DrawTextureRect(x, y, z, sx, -sy, sz, 1.0f, 1.0f, 1.0f, 1.0f,
			i->getIdx());
	}
}

void MenuScene::destroy(){
	for (auto& i : lists)
		delete i;
	lists.clear();
	delete renderer;
}
