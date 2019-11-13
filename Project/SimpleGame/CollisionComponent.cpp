#include "stdafx.h"
#include "CollisionComponent.h"
#include "Object.h"
void ICollisionComponent::process(Object * o){
	collisionBox = box(o->getPos(), o->getVolume());
}

void ICollisionComponent::pushCollision(Object * o){
	collisionObject.push_back(o);
}

const std::list<Object*>& ICollisionComponent::getCollisionObject() const {
	return collisionObject;
}

void ICollisionComponent::clear(){
	collisionObject.clear();
}

box ICollisionComponent::getCollisionBox(){
	return collisionBox;
}

void ICollisionComponent::setCollisionBox(Object* o){

}


