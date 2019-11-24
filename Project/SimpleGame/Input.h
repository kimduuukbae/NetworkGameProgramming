#pragma once
#define D_INPUT Input::instance()
class Input {
public:
	static Input* instance();
	Input();
	~Input();
	bool isKeyDown(int key);
	bool isKeyUp(int key);
	bool isKeyOverlap(int key);
	bool isMouseDown();
	void setDeltaTime(float t);
	float getDeltaTime();
	short mx, my;
private:
	float deltaTime;
};

void KeyDownInput(unsigned char key, int x, int y);
void KeyUpInput(unsigned char key, int x, int y);
void SpecialKeyDownInput(int key, int x, int y);
void SpecialKeyUpInput(int key, int x, int y);
void MouseInput(int button, int state, int x, int y);

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F