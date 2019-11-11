#include "stdafx.h"
#include "Object.h"
#include "IComponent.h"
#include <type_traits>
Object::~Object(){
	for (auto& i : components) 
		delete i;
	components.clear();
}
void Object::setPos(float x, float y, float z){
	setValue(x, y, z, position);
}

void Object::setVolume(float x, float y, float z){
	setValue(x, y, z, volume);
}

void Object::setColor(float r, float g, float b, float a){
	stColor.r = r;
	stColor.g = g;
	stColor.b = b;
	stColor.a = a;
}

value Object::getPos(){
	return value{ position };
}

value Object::getVolume(){
	return volume;
}

color Object::getColor(){
	return stColor;
}

void Object::setVelocity(float x, float y, float z){
	velocity = value{ x,y,z };
}

value Object::getVelocity() {
	return velocity;
}

void Object::update(float deltaTime){
	for (auto& i : components)
		i->process(this);
}

void Object::setIdx(int idx){
	pngIdx = idx;
}

int Object::getIdx(){
	return pngIdx;
}

void Object::setValue(float x, float y, float z, value & v) {
	v.x = x;
	v.y = y;
	v.z = z;
}

void delObject(Object* o) {
	delete o;
}