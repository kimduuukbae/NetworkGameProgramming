#pragma once
#include "Scene.h"
class ObjectManager;
class MenuScene : public Scene {
public:
	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	Renderer* renderer;
	ObjectManager* o;
};