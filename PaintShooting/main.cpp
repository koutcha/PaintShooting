#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define _USE_MATH_DEFINES
#include <iostream>
#include <memory>
#include<thread>

#include "Window.h"
#include "Audio.h"

#include "GameMain.h"

//load stb_image header only library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;



int main(void)
{
	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "Can't initilize GLFW" << std::endl;
		return 1;
	}
	// register tha program for exit processing

	atexit(glfwTerminate);


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//FXAA
	glfwWindowHint(GLFW_SAMPLES, 4);

	//set up glfw window
	std::shared_ptr<Window> window(std::make_shared<Window>());
	//set up openAL audio
	std::unique_ptr<Sound::Audio> audio(std::make_unique<Sound::Audio>());

	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	

	//fps setting 
	double FPS = 60.0;
	double currentTime = 0.0, lastTime = 0.0, nextTime = 0.0;
	double interval = 1.0 / FPS;
	double dSleep = 0.0;
	int sleep = 0;
	lastTime = 0;
	nextTime = interval;
	double delta = 0;
	//set up gameMain
	std::unique_ptr<GameMain> gameMain(std::make_unique<GameMain>(interval, window));



	while (window->shouldClose() == GL_FALSE)
	{


		currentTime = glfwGetTime();
		if (currentTime >= nextTime)
		{
			//cout << currentTime - lastTime << endl;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, window->getSize()[0], window->getSize()[1]);
			delta += interval;
			lastTime = currentTime;
			nextTime = currentTime + interval;

			gameMain->updateGame();

			window->swapBuffers();
			window->pollEvents();

		}
		else 	if ((dSleep = nextTime - currentTime) > 0)
		{
			sleep = static_cast<int>(dSleep * 1000);
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
		}


	}


	return 0;
}