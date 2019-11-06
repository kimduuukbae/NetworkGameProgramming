#pragma once
class Vector3D;
class Object;
class Renderer;

class Scene {
public:
	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;

	Scene() = default;
	virtual ~Scene() = default;
private:
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
};

// 모든 Scene 은 이 Header 를 inheritance 하여 사용할 수 있음
