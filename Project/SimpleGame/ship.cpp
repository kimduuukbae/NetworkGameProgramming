#include "stdafx.h"
#include "ship.h"
#include "PhysicsComponent.h"

Ship::Ship(){
	addComponent<IPhysicsComponent>();
	physics = getComponent<IPhysicsComponent>();
}

void Ship::update(float deltaTime){
	
}
