#pragma once
#include "Scene.h"
#include <vector>
#include "ServerDevice.h"
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
	ServerDevice serverDevice;
	float shootDelay = 0.0f;
	int count;
};