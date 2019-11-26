#include "Object.h"
#include <cmath>
#include <iostream>
using namespace std;
Object::Object(value pos, value dir, E_OBJECT_TYPE e)
	:
	position{pos},
	direction{dir},
	velocity(value{0.0f,0.0f,0.0f}),
	type{e},
	gearTime{0.0},
	pushType{E_NONE}
{}

void Object::update(double deltaTime){
	position = position + velocity * deltaTime;
	if (pushType != E_NONE) {
		gearTime += deltaTime;
		if (gearTime > 1.0f) {
			if (pushType == E_PUSH)
				increaseSpeed();
			else if (pushType == E_RELEASED)
				decreaseSpeed();
			gearTime = 0.0f;
		}
	}
}

void Object::increaseSpeed(){
	Vector3D v = velocity;
	v += direction;
	velocity = v;
}

void Object::decreaseSpeed(){
	Vector3D v1 = velocity;
	Vector3D v2 = velocity;
	if (v1.size() < 0.1f)
		pushType = E_NONE;
	else {
		v2.normalize();
		v1 = v1 - v2;
		auto[x, y, z] = v1.getValue();
		v1.setX((x > 0.9f) ? x : (x < -0.9f) ? x : 0.0f);
		v1.setY((y > 0.9f) ? y : (y < -0.9f) ? y : 0.0f);

		velocity = v1;
	}
}

void Object::rotation(float r){
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

E_OBJECT_TYPE Object::getType(){
	return type;
}

value Object::getPos(){
	return position.getValue();
}
