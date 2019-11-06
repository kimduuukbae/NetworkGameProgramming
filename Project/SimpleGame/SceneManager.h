#pragma once
#include <vector>
#include <string>
#include "structure.h"
class Scene;
class Renderer;
class SceneManager {
public:
	static SceneManager* instance();
	SceneManager();
	~SceneManager();
	void render();
	void update(float deltaTime);
	void commit(const char* name, Scene* ps);
private:
	struct sceneCluster {
		Scene* scene;
		std::string sceneName;
		sceneCluster(const char* name, Scene* p) :sceneName{ name }, scene{ p } {}
	};
	std::vector<sceneCluster> aScene;	// 모든 Scene들의 집합
	Scene* curScene{ nullptr };	// 현재 Scene
};