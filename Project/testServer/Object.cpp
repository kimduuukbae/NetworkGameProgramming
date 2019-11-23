#include "Object.h"
#include <cmath>
Object::Object(value pos, value dir, E_OBJECT_TYPE e)
	:
	position{pos},
	direction{dir},
	type{e}
{}

void Object::move(){
	velocity += direction;
}

void Object::increaseSpeed()
{
}

void Object::decreaseSpeed(){
	Vector3D v = velocity;
	v.normalize();
	velocity = velocity - v;
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

E_OBJECT_TYPE Object::getType(){
	return type;
}
