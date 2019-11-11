#include "stdafx.h"
#include "menuScene.h"
#include "structure.h"
#include "Define.h"
#include "Object.h"
#include "Renderer.h"
#include "ship.h"

void MenuScene::init(){
	renderer = new Renderer(500, 500);
	if (!renderer->IsInitialized())
		return;
	lists.push_back(addObject<Ship>(value{ 0.0f,0.0f,0.0f }, color{ 1.0f,1.0f,1.0f,1.0f },
		value{ 100.0f,100.0f,100.0f }, value{ 0.0f,0.0f,0.0f }, renderer->GenPngTexture("texture/title.png")));
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
