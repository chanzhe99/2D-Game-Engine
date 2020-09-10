#include <GLFW/glfw3.h>
#include <Windows.h>
#include <gl/GLU.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
//#include <chrono>
#include "Game.h"
#include "FMod.h"

//Fmod values
#include <fmod.hpp>
#include <fmod_errors.h>

const int RESOLUTION_X = 1280;
const int RESOLUTION_Y = 900;


void OnWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Use ortho 2D view
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(void)
{
	srand(time(NULL));

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, OnWindowResized);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	OnWindowResized(window, RESOLUTION_X, RESOLUTION_Y);

	Matrix matrix(Matrix::makeIdentityMatrix());

	Game ShmupGame(Vector2(RESOLUTION_X, RESOLUTION_Y));
	ShmupGame.Start();


	glfwSwapInterval(0); // enable/disable vsync

	double LastFrameTime = glfwGetTime();
	double MaxFPS = 240.00;
	double MinDeltaTime = 1.0 / MaxFPS;

	initFmod();

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		double ThisFrameTime = glfwGetTime();
		double DeltaTime = ThisFrameTime - LastFrameTime;

		//============================================================
		int MinSleepTimeTH = 10;
		// Conservatively asking the system to sleep
		int SleepTime = (MinDeltaTime - DeltaTime) * 1000 - MinSleepTimeTH;
		if (SleepTime >= MinSleepTimeTH)
			Sleep(SleepTime);

		// Sleep time is no precise, use loop to "waste" extra cpu cycles
		while (DeltaTime < MinDeltaTime)
			DeltaTime = glfwGetTime() - LastFrameTime;
		//============================================================

		LastFrameTime = glfwGetTime();

		float FPS = 1.0 / DeltaTime;
		char str[32];
		sprintf_s(str, "%f, %f", FPS, (float)DeltaTime);
		glfwSetWindowTitle(window, str);
		
		m_fmodSystem->update();
		ShmupGame.UpdateInput(window, DeltaTime);
		ShmupGame.Update(DeltaTime);
		ShmupGame.Draw();


		glfwSwapBuffers(window);
		glfwPollEvents();

		
	}

	glfwTerminate();
	return 0;
}