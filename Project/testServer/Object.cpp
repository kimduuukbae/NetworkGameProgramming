#include "Object.h"
#include <cmath>
#include <iostream>
using namespace std;
Object::Object(value pos, value dir, value vol, E_OBJECT_TYPE e)
	:
	position{pos},
	direction{dir},
	velocity(value{0.0f,0.0f,0.0f}),
	type{e},
	gearTime{0.0},
	pushType{E_NONE},
	volume{ box{vol.x / 2, vol.x / 2, vol.y / 2, vol.y / 2} },
	ancesteridx {-1},
    HealthPoint{100},
	maxSpeed{15.0f},
	deleteIt {false}
{}

void Object::update(double deltaTime){
	position = position + velocity * deltaTime;
	position.setX(clamp(-700.0f, position.getX(), 700.0f));
	position.setY(clamp(-400.0f, position.getY(), 400.0f));
	if (pushType != E_NONE) {
		gearTime += deltaTime;
		if (gearTime > 0.2f) {
			if (pushType == E_PUSH)
				increaseSpeed();
			else if (pushType == E_RELEASED)
				decreaseSpeed();
			Vector3D v = velocity;
			v.setX(clamp(-maxSpeed, v.getX(), maxSpeed));
			v.setY(clamp(-maxSpeed, v.getY(), maxSpeed));
			velocity = v;
			gearTime = 0.0f;
		}
	}
	if (type == E_BULLET & velocity.size() < 0.1f)
		setDelete();
}

void Object::increaseSpeed(){
	Vector3D v = velocity + direction;
	velocity = v;
}

void Object::decreaseSpeed(){
	Vector3D v1 = velocity;
	Vector3D v2 = velocity;
	if (v1.size() < 0.1f) {
		pushType = E_NONE;
		v1.setX(0.0f);
		v1.setY(0.0f);
	}
	else {
		v2.normalize();
		v1 = v1 - v2;
		auto[x, y, z] = v1.getValue();
		v1.setX((x > 0.9f) ? x : (x < -0.9f) ? x : 0.0f);
		v1.setY((y > 0.9f) ? y : (y < -0.9f) ? y : 0.0f);
	}
	velocity = v1;
}

void Object::rotation(float r){
	Vector3D v = velocity;
	v.setX((std::cos(r) * v.getX()) - (std::sin(r) * v.getY()));
	v.setY(std::sin(r) * v.getX() + std::cos(r) * v.getY());
	velocity = v;

	rad += r;
	if (rad < -12.56f || rad > 12.56f)
		rad = 0.0f;
	auto[x, y, z] = direction.getValue();
	x = std::cos(rad);
	y = std::sin(rad);

	
	direction = Vector3D{ x,y,0.0f };
}

void Object::addSpeed(float f){
	if (f > 0.1f)
		pushType = E_PUSH;
	else
		pushType = E_RELEASED;
}

void Object::setVelocity(float x, float y, float z){
	velocity = value{ x,y,z };
}

void Object::setIdx(int i){
	idx = i;
}

void Object::setAncesterIdx(int i){
	ancesteridx = i;
}

void Object::manageHp(int damage)
{
    HealthPoint -= damage;
    if (HealthPoint > 100)
        HealthPoint = 100;
}

void Object::setMaxSpeed(float maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

float Object::getMaxSpeed()
{
	return maxSpeed;
}

void Object::setDamage(int damage)
{
	this->damage += damage;
}

int Object::getDamage()
{
	return damage;
}

int Object::getHp()
{
    return HealthPoint;
}

int Object::getAncester(){
	return ancesteridx;
}

int Object::getIdx(){
	return idx;
}

void Object::setDelete(){
	deleteIt = true;
}

bool Object::getDelete(){
	return deleteIt;
}

box Object::getBox(){
	auto[x, y, z] = position.getValue();
	return { x - volume.left, x + volume.right, y + volume.top, y - volume.bottom };
}

E_OBJECT_TYPE Object::getType(){
	return type;
}

value Object::getPos(){
	return position.getValue();
}

void Object::setPos(value pos)
{
	position.setX(pos.x);
	position.setY(pos.y);
	position.setZ(pos.z);
}

value Object::getVelocity(){
	return velocity.getValue();
}
