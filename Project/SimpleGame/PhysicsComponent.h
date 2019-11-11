#pragma once
#include "IComponent.h"
class IPhysicsComponent : public IComponent {
public:
	IPhysicsComponent() = default;
	void process(Object* o) override;
};