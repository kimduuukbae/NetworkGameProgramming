#include "stdafx.h"
#include "SceneManager.h"
#include "Dependencies\freeglut.h"
#include "Scene.h"

static SceneManager* inst = nullptr;
SceneManager* SceneManager::instance(){
	if (inst == nullptr)
		inst = new SceneManager();
	
	return inst;
}

SceneManager::SceneManager(){

}

SceneManager::~SceneManager(){
	for (auto& i : aScene) {
		i.scene->destroy();
		delete i.scene;
	}
	aScene.clear();
	delete this;
}
void SceneManager::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glClearColor(0.251f, 0.455f, 0.961f, 1.0f);
	glClearColor(0.180f, 0.478f, 0.780f, 1.0f);

	curScene->draw();
}
void SceneManager::update(float deltaTime) {
	curScene->update(deltaTime);
}

void SceneManager::commit(const char * name, Scene * ps){
	aScene.emplace_back(name, ps);
	if(curScene == nullptr)
		curScene = aScene[0].scene;
	aScene.back().scene->init();
}
