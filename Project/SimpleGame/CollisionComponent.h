#pragma once
#include "IComponent.h"
#include <list>
class ICollisionComponent : public IComponent{
public:
	ICollisionComponent() = default;
	void process(Object* o) override;

private:
	std::list<Object*> collisionObject;
};