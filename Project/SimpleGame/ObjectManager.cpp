#include "stdafx.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include "Renderer.h"
static ObjectManager* inst = nullptr;
ObjectManager* ObjectManager::instance() {
	if (inst == nullptr)
		inst = new ObjectManager;

	return inst;
}
ObjectManager::ObjectManager() {
	renderer = new Renderer(1600, 900);
	garbageTime = 0.0f;
	garbageTimePlan = 15.0f;
}
ObjectManager::~ObjectManager() {
	delete this;
}

void ObjectManager::destroy(){
	for (auto& i : objects)
		delete i;
	objects.clear();
	collisionObjects.clear();
}

void ObjectManager::draw(){
	for (auto& i : objects) {
		if (!i->getDelete()) {
			auto[x, y, z] = i->getPos();
			auto[sx, sy, sz] = i->getVolume();
			auto[r, g, b, a] = i->getColor();
			renderer->DrawTextureRect(x, y, z, sx, -sy, sz, r, g, b, a,
				i->getPngIdx(),i->getDegree());
		}
	}
}

void ObjectManager::update(float dt){
	for (auto& i : objects)
		i->update(dt);
	updateCollision();
	garbageTime += dt;
	if (garbageTime > garbageTimePlan) {
		garbageTime = 0.0f;
		garbageCollection();
	}
}

std::vector<Object*>& ObjectManager::getObjects(){
	return objects;
}

Object* ObjectManager::getObject(int idx){
	return objects[idx];
}

const std::list<Object*>* ObjectManager::getCollisionObjectList(Object * o) const {
	if (auto comp = o->getComponent<ICollisionComponent>(); comp != nullptr)
		return &comp->getCollisionObject();
	return nullptr;
}

void ObjectManager::pushCollisionObject(Object* o){
	if (o->getComponent<ICollisionComponent>() != nullptr) 
		collisionObjects.push_back(o);
}
void ObjectManager::updateCollision(){
	for (auto it1 = collisionObjects.begin(); it1 != collisionObjects.end(); ++it1) {
		(*it1)->getComponent<ICollisionComponent>()->clear();
		if ((*it1)->getDelete())
			continue;
		for (auto it2 = collisionObjects.begin(); it2 != collisionObjects.end(); ++it2) {
			if (it1 == it2 || (*it2)->getDelete())
				continue;
			if (auto comp = (*it1)->getComponent<ICollisionComponent>();
				AABBCollision(
				(*it2)->getComponent<ICollisionComponent>()->getCollisionBox(), comp->getCollisionBox())) {
				comp->pushCollision(*it2);
			}
		}
	}
}
int ObjectManager::generate(const char * s){
	return renderer->GenPngTexture(const_cast<char*>(s));
}
template <typename T>
void pointerSwap(T& t1, T& t2) {
	Object* tmp = *t1;
	*t1 = *t2;
	*t2 = tmp;
}
void ObjectManager::garbageCollection() {

	for (auto it = collisionObjects.begin(); it != collisionObjects.end();) {
		if ((*it)->getDelete()) 
			it = collisionObjects.erase(it);
		else
			++it;
	}
	int count = 1;

	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if ((*it) == nullptr)
			break;
		if ((*it)->getDelete()) {
			pointerSwap(it, objects.end() - count);
			delete objects[objects.size() - count];
			objects[objects.size() - count] = nullptr;
			++count;
		}
	}
	if (count > 1) {
		auto it = objects.end() - (count-1);
		objects.erase(it, objects.end());
	}
}

void ObjectManager::setGarbageTime(float time) {
	garbageTimePlan = time;
}