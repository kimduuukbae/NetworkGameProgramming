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
	renderer = new Renderer(1024, 1024);
}
ObjectManager::~ObjectManager() {
	delete this;
}

void ObjectManager::destroy(){
	for (auto& i : objects)
		delete i;
	objects.clear();
}

void ObjectManager::draw(){
	for (auto& i : objects) {
		auto[x, y, z] = i->getPos();
		auto[sx, sy, sz] = i->getVolume();
		renderer->DrawTextureRect(x, y, z, sx, -sy, sz, 1.0f, 1.0f, 1.0f, 1.0f,
			i->getIdx());
	}
}

void ObjectManager::update(float dt){
	for (auto& i : objects)
		i->update(dt);
}

std::vector<Object*>* ObjectManager::getObject(){
	return &objects;
}

void ObjectManager::updateCollisionObject(Object* o){
	if (o->getComponent<ICollisionComponent>() != nullptr)
		collisionObjects.push_back(o);
}
int ObjectManager::generate(const char * s){
	return renderer->GenPngTexture(const_cast<char*>(s));
}
void ObjectManager::garbageCollection() {
	int count = 1;
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if ((*it)->getDelete()) {
			std::swap(it, objects.end() - count);
			delete objects[objects.size() - count];
			++count;
		}
	}
	objects.erase(objects.begin() + objects.size() - count, objects.end());
}