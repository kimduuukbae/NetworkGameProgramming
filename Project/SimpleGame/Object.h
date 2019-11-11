#pragma once
#include "structure.h"
#include <vector>
#include <typeinfo>
class IComponent;

class Object {
public:
	Object() = default;
	virtual ~Object();
	void setPos(float x, float y, float z);
	value getPos();

	void setVolume(float x, float y, float z);
	value getVolume();

	void setMass(float m);
	float getMass();

	void setColor(float r, float g, float b, float a);
	color getColor();

	void setVelocity(float x, float y, float z);
	value getVelocity();

	virtual void update(float deltaTime);
	void setIdx(int idx);

	int getIdx();

	template <typename T>
	T* getComponent() {
		for (auto& i : components)
			if (typeid(T) == typeid(*i)) 
				return dynamic_cast<T*>(i);
		return nullptr;
	}
	template <typename T>
	void addComponent() {
		components.push_back(new T());
	}
private:
	value position;
	value volume;
	value velocity;
	float mass;

	color stColor;
	void setValue(float x, float y, float z, value& v);

	int pngIdx;
	std::vector<IComponent*> components;
	
};
template <typename T>
Object* addObject(const value& pos, const color& c, const value& volume,
	const value& velocity, float fric, float mass);
