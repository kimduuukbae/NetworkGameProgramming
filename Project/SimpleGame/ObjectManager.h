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

	/* o 라는 오브젝트와 충돌된 모든 오브젝트를 list 형태로 반환합니다.*/
	const std::list<Object*>* getCollisionObjectList(Object* o) const;
	/* idx 를 가지고 T* 형태로 뱉어냄, 실패시 nullptr */
	template <typename T>
	T* getObject(int idx) {
		return dynamic_cast<T*>(objects[idx]);
	}
	template <typename T>
	int addObject(const value& pos, const color& c, const value& volume,
		const value& velocity, const char* s) {
		Object* o = new T();
		o->setPos(pos.x, pos.y, pos.z);
		o->setVolume(volume.x, volume.y, volume.z);
		o->setColor(c.r, c.g, c.b, c.a);
		o->setVelocity(velocity.x, velocity.y, velocity.z);
		o->setPngIdx(generate(s));
		o->setIdx(objects.size());
		objects.push_back(o);
		pushCollisionObject(o);
		return o->getIdx();
	}
	void setGarbageTime(float time);
	void garbageCollection();
private:
	std::vector<Object*> objects;
	std::list<Object*> collisionObjects;
	void pushCollisionObject(Object* o);
	void updateCollision();
	int generate(const char* s);
	Renderer* renderer;

	float garbageTime;
	float garbageTimePlan;
};

