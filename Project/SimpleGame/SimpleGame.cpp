/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include <crtdbg.h>
#include "menuScene.h"


int elapsedTime{};
void RenderScene(int temp){
	float time = float(glutGet(GLUT_ELAPSED_TIME) - elapsedTime) / 1000.0f;
	D_INPUT->setDeltaTime(time);
	D_SCENE->update(time);
	D_SCENE->render();
	glutSwapBuffers();
	elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(16, RenderScene, 16);
}
void Idle(void){
	
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Game Software Engineering KPU");
	
	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
		std::cout << " GLEW Version is 3.0\n ";
	else
		std::cout << "GLEW 3.0 not supported\n ";

	D_SCENE->commit("Menu", new MenuScene);

	glutDisplayFunc(Idle);
	glutIdleFunc(Idle);
	glutMouseFunc(MouseInput);
	elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(16, RenderScene, 16);
	//atexit() GLUT �� OS ���� resource free �� �ñ�� ������, memory leak �� ������. atexit�� ���� ���� �Լ��� �� ȣ������
	glutMainLoop();
	printf("EXIT");

    return 0;
}

