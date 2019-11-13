#pragma once
#include "IComponent.h"
#include <list>
#include "structure.h"
class ICollisionComponent : public IComponent{
public:
	ICollisionComponent() = default;
	void process(Object* o) override;
	void pushCollision(Object* o);
	const std::list<Object*>& getCollisionObject() const;
	void clear();
	box getCollisionBox();
	void setCollisionBox(Object* o);
private:
	std::list<Object*> collisionObject;
	box collisionBox;
};