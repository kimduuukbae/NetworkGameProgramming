#pragma once
#include "structure.h"
#include <vector>
#include <typeinfo>
class IComponent;
enum E_TYPE {
	E_SHIP,
	E_BULLET,
	E_ITEM,
	E_REEF,
	E_WIND
};
class Object {
public:
	Object() = default;
	Object(Object&& o) = default;
	virtual ~Object();
	void setPos(float x, float y, float z);
	value getPos();

	void setVolume(float x, float y, float z);
	value getVolume();

	void setColor(float r, float g, float b, float a);
	color getColor();

	void setVelocity(float x, float y, float z);
	void setVelocity(const Vector3D& v);
	value getVelocity();

	void addVelocityX(float x);
	void addVelocityY(float y);
	void addVelocityZ(float z);

	virtual void update(float deltaTime);

	void setPngIdx(int idx);
	int getPngIdx();

	void setIdx(int idx);
	int getIdx();
	
	void setDelete();
	bool getDelete();

	void setDegree(float f);
	float getDegree();

	void setType(E_TYPE e);

	void setLive(bool flag);
	bool getLive();

	void setCollobject(bool flag);
	bool getCollobject();
	E_TYPE getType();
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
	template <typename T>
	static T* getObjectCast(Object* o) {
		return dynamic_cast<T*>(o);
	}
private:
	value position;
	value volume;
	value velocity;

	color stColor;
	void setValue(float x, float y, float z, value& v);

	int pngIdx;
	int objectIdx;
	std::vector<IComponent*> components;
	bool deleteIt;
	bool live{ true };
	bool coll{ false };
	float degree;
	E_TYPE myType;
};