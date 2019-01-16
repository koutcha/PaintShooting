#pragma once
#include <iostream>
#include <array>
#include "GL/glew.h"
#include "GLFW\glfw3.h"

//Window-rerated process
class Window
{
public:
	Window(int width = 1280, int height = 720, const char *title = "window");
	virtual ~Window();
	const GLfloat *getSize() const { return size; }
	GLfloat getScale() const { return scale; }
	const GLfloat*getLocation()const { return location; }
	int shouldClose() const;
	void swapBuffers();

	void pollEvents();

	bool keyOn(int glfw_keyID)const;
	bool keyTriggered(int glfw_keyID)const;
	//getMouseButtonSatte
	bool buttonOn(int glfw_mouseButtonID)const;
	
private:
	//window handle
	GLFWwindow *const window;
	//window size
	GLfloat size[2];
	//the scale of device cordinate system for world cordinate system
	GLfloat scale;
	//cursor's position at device cordinate system
	GLfloat location[2];

	int keyStatus;

	static void resize(GLFWwindow* const window, int width, int height);
	static void wheel(GLFWwindow* window, double x, double y);
	static void keyboard(GLFWwindow* const window, int key, int scancode, int action, int mods);
};