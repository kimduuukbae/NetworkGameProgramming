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

	void setColor(float r, float g, float b, float a);
	color getColor();

	void setVelocity(float x, float y, float z);
	value getVelocity();

	virtual void update(float deltaTime);

	void setIdx(int idx);
	int getIdx();
	
	void setDelete();
	bool getDelete();
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

	color stColor;
	void setValue(float x, float y, float z, value& v);

	int pngIdx;
	std::vector<IComponent*> components;
	bool deleteIt;
};
