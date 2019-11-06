#pragma once
#include "Scene.h"
#include <list>

class MenuScene : public Scene {
public:
	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	std::list<Object*> lists;
	Renderer* renderer;
};