#pragma once
#include "Object.h"

class Wind : public Object
{
	Vector3D wind;
public:
	Wind();
	~Wind() = default;
	void setWind(Vector3D w);
	void collideWind(Object* obj);
};

