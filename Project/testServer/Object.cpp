#include "Object.h"

Object::Object(value pos, value dir, E_OBJECT_TYPE e)
	:
	position{pos},
	direction{dir},
	type{e}
{}

E_OBJECT_TYPE Object::getType(){
	return type;
}
