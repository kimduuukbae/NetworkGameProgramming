#pragma once
#include <vector>
#include "Object.h"
class Renderer;
#define D_OBJECT ObjectManager::instance()
class ObjectManager {
public:
	static ObjectManager* instance();
	ObjectManager();
	~ObjectManager();
	void destroy();
	void draw();
	void update(float dt);
	std::vector<Object*>* getObject();
	void garbageCollection();
	template <typename T>
	void addObject(const value& pos, const color& c, const value& volume,
		const value& velocity, const char* s) {
		Object* o = new T();
		o->setPos(pos.x, pos.y, pos.z);
		o->setVolume(volume.x, volume.y, volume.z);
		o->setColor(c.r, c.b, c.g, c.r);
		o->setVelocity(velocity.x, velocity.y, velocity.z);
		o->setIdx(generate(s));
		objects.push_back(o);
		updateCollisionObject(o);
	}
private:
	std::vector<Object*> objects;
	std::vector<Object*> collisionObjects;
	void updateCollisionObject(Object* o);
	int generate(const char* s);
	Renderer* renderer;
};

