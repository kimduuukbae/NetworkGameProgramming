#pragma once
#include "Scene.h"
#include <vector>
#include "Object.h"
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
	std::vector<Object*>* v;
};