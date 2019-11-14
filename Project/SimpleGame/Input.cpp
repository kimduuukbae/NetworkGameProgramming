#include "stdafx.h"
#include "Input.h"
#include "Dependencies\freeglut.h"
static Input* inst = nullptr;
bool keyFlag[256];
Input * Input::instance(){
	if (inst == nullptr)
		inst = new Input();
	return inst;
}

Input::Input(){
	for (int i = 0; i < 256; ++i)
		keyFlag[i] = false;
}

Input::~Input(){
	delete this;
}

bool Input::isKeyDown(int key){
	if (GetAsyncKeyState(key) & 0x8000) {
		if (!keyFlag[key])
			return keyFlag[key] = true;
	}
	return false;
}

bool Input::isKeyUp(int key){
	if (GetAsyncKeyState(key) & 0x8000)
		keyFlag[key] = true;
	else
		if (keyFlag[key])
			return !(keyFlag[key] = false);
	return false;
}

bool Input::isKeyOverlap(int key){
	return keyFlag[key];
}

void Input::setDeltaTime(float t){
	deltaTime = t;
}

float Input::getDeltaTime(){
	return deltaTime;
}
void MouseInput(int button, int state, int x, int y) {
	
}