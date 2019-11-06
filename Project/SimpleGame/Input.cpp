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
	return keyFlag[key];
}

bool Input::isKeyUp(int key){
	return !keyFlag[key];
}

bool Input::isKeyOverlap(int key){
	return keyFlag[key];
}

void KeyDownInput(unsigned char key, int x, int y) {
	keyFlag[key] = true;
}
void KeyUpInput(unsigned char key, int x, int y) {
	keyFlag[key] = false;
}
void SpecialKeyDownInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		keyFlag[VK_LEFT] = true;
		break;
	case GLUT_KEY_RIGHT:
		keyFlag[VK_RIGHT] = true;
		break;
	case GLUT_KEY_UP:
		keyFlag[VK_UP] = true;
		break;
	case GLUT_KEY_DOWN:
		keyFlag[VK_DOWN] = true;
		break;
}
}
void SpecialKeyUpInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		keyFlag[VK_LEFT] = false;
		break;
	case GLUT_KEY_RIGHT:
		keyFlag[VK_RIGHT] = false;
		break;
	case GLUT_KEY_UP:
		keyFlag[VK_UP] = false;
		break;
	case GLUT_KEY_DOWN:
		keyFlag[VK_DOWN] = false;
		break;
	}
}
void MouseInput(int button, int state, int x, int y) {
	//
}