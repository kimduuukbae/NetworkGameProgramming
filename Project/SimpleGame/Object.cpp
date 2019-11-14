#include "stdafx.h"
#include "Object.h"
#include "IComponent.h"
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

void Object::setVelocity(const Vector3D & v){
	velocity.x += v.getX();
	velocity.y += v.getY();
	velocity.z += v.getZ();
}

value Object::getVelocity() {
	return velocity;
}

void Object::addVelocityX(float x){
	velocity.x += x;
}

void Object::addVelocityY(float y){
	velocity.y += y;
}

void Object::addVelocityZ(float z){
	velocity.z += z;
}



void Object::update(float deltaTime){
	for (auto& i : components)
		i->process(this);
}

void Object::setPngIdx(int idx){
	pngIdx = idx;
}

int Object::getPngIdx(){
	return pngIdx;
}

void Object::setIdx(int idx){
	objectIdx = idx;
}

int Object::getIdx(){
	return objectIdx;
}

void Object::setDelete(){
	deleteIt = true;
}

bool Object::getDelete(){
	return deleteIt;
}

void Object::setType(E_TYPE e){
	myType = e;
}

E_TYPE Object::getType(){
	return myType;
}

void Object::setValue(float x, float y, float z, value & v) {
	v.x = x;
	v.y = y;
	v.z = z;
}