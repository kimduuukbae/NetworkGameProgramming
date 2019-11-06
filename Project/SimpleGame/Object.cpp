#include "stdafx.h"
#include "Object.h"
#include "Define.h"


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

void Object::setMass(float m){
	mass = m;
}

float Object::getMass(){
	return mass;
}

color Object::getColor(){
	return stColor;
}

void Object::update(float deltaTime){

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
template <typename T>
void addObject(const value& pos, const color& c, const value& volume,
	const value& velocity, float fric, float mass) {
	Object* o = new T();
	o->setPos(pos.x, pos.y, pos.z);
	o->setVolume(volume.x , volume.y, volume.z);
	o->setColor(c.r, c.b, c.g, c.r);
	o->setFriction(fric);
	o->setVelocity(velocity.x, velocity.y, velocity.z);
	o->setMass(mass);

	v.push_back(o);
}
void delObject(Object* o) {
	delete o;
}