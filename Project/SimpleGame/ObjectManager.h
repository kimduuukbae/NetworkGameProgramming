#pragma once
#include <vector>
#include <list>
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
	std::vector<Object*>& getObjects();
	Object* getObject(int idx);
	void garbageCollection();
	template <typename T>
	int addObject(const value& pos, const color& c, const value& volume,
		const value& velocity, const char* s) {
		Object* o = new T();
		o->setPos(pos.x, pos.y, pos.z);
		o->setVolume(volume.x, volume.y, volume.z);
		o->setColor(c.r, c.g, c.b, c.a);
		o->setVelocity(velocity.x, velocity.y, velocity.z);
		o->setPngIdx(generate(s));
		o->setIdx(objects.size() - 1);
		objects.push_back(o);
		pushCollisionObject(o);
		return o->getIdx();
	}
	/* o ��� ������Ʈ�� �浹�� ��� ������Ʈ�� list ���·� ��ȯ�մϴ�.*/
	const std::list<Object*>& getCollisionObjectList(Object* o) const;
private:
	std::vector<Object*> objects;
	std::list<Object*> collisionObjects;
	void pushCollisionObject(Object* o);
	void updateCollision();
	int generate(const char* s);
	Renderer* renderer;
};

