// TrainingFramework.cpp : Defines the entry point for the console application.

#include "../TrainingFramework/stdafx.h"
#define SDL_MAIN_HANDLED
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "../TrainingFramework/GameObject/Vertex.h"
#include "../TrainingFramework/GameObject/Model.h"
#include "../TrainingFramework/GameObject/Texture.h"
#include "../TrainingFramework/GameObject/Shaders.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Camera.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameState/GSMachine.h"
#include "../TrainingFramework/GameState/GSBase.h"
#include "Globals.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

int KeyPressed = 0;
int Init ( ESContext *esContext ) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	esContext->running = true;
	ResourceManager::GetInstance()->LoadResource("../Resources/RM.txt");
	SceneManager::GetInstance()->LoadScene("../Resources/SM.txt");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	GSMachine::GetInstance()->PushState(STATE_INTRO);
	return 0;
}

void Draw ( ESContext *esContext ) {
	glClear(GL_COLOR_BUFFER_BIT);
	GSMachine::GetInstance()->Draw();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime ) {
	esContext->running = GSMachine::GetInstance()->IsRunning();
	if (esContext->running)
	{
		GSMachine::GetInstance()->PerformStateChange();
		GSMachine::GetInstance()->Update(deltaTime);
	}
}

void CleanUp() {
	ResourceManager::FreeInstance();
	SceneManager::FreeInstance();
	GSMachine::FreeInstance();
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed) {
	GSMachine::GetInstance()->GetCurrentState()->HandleKeyEvents(key, bIsPressed);
}

void MouseClick(ESContext* esContext, int x, int y, bool isPressed) 
{
	if (isPressed) printf("%d %d\n", x, y);
	GSMachine::GetInstance()->GetCurrentState()->HandleTouchEvents(x, y, isPressed);
}

void OnMouseMove(ESContext* esContext, GLint x, GLint y) 
{
	GSMachine::GetInstance()->GetCurrentState()->HandleMouseMoveEvents(x, y);
}

int _tmain(int argc, _TCHAR* argv[]) {
	ESContext esContext;

    esInitContext ( &esContext );
	esCreateWindow ( &esContext, "PUZZLE!!!", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseFunc(&esContext, MouseClick);
	esRegisterMouseMoveFunc(&esContext, OnMouseMove);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();
	printf("Press any key...\n");

	return 0;
}