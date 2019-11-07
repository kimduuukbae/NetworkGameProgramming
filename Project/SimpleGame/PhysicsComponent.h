#pragma once
#include "IComponent.h"
#include "structure.h"
class IPhysicsComponent : public IComponent {
public:
	IPhysicsComponent() = default;
	void process(Object* o) override;
	void addForce(float x, float y, float z);
private:
	Vector3D power;
};