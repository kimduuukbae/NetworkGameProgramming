#pragma once
#include "Scene.h"
#include <vector>

class ObjectManager;
class Object;
class MenuScene : public Scene {
public:
	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	Renderer* renderer;
	ObjectManager* o;

	std::vector<Object*> v;

	float windChangeCoolTime;
	float itemCreationCoolTime;
	float defaultWindCoolTime = 30.f;
	float defaultItemCoolTime = 20.f;
	float shootDelay = 0.0f;
	int count;
};