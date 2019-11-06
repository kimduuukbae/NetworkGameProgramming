#pragma once

class Input {
public:
	static Input* instance();
	Input();
	~Input();
	bool isKeyDown(int key);
	bool isKeyUp(int key);
	bool isKeyOverlap(int key);
private:
};

void KeyDownInput(unsigned char key, int x, int y);
void KeyUpInput(unsigned char key, int x, int y);
void SpecialKeyDownInput(int key, int x, int y);
void SpecialKeyUpInput(int key, int x, int y);
void MouseInput(int button, int state, int x, int y);