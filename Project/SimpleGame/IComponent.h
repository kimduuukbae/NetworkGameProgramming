#pragma once
class Object;

class IComponent {
public:
	IComponent() = default;
	virtual ~IComponent() = default;
	virtual void process(Object* o) = 0;
};