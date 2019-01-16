#include "Window.h"

Window::Window(int width, int height, const char * title)
	:window(glfwCreateWindow(width, height, title, nullptr, nullptr))
	, scale(100.0f), location{ 0,0 }, keyStatus(GLFW_RELEASE)
{
	if (window == nullptr) {
		//couldn't create window
		std::cerr << "Can't creat GLFW window" << std::endl;
		exit(1);
	}

	//target current window for process
	glfwMakeContextCurrent(window);

	//GLEW init
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//failed in intializing glew
		std::cerr << "Can't intialize GLEW" << std::endl;
		exit(1);
	}
	//wait V sync
	glfwSwapInterval(1);


	//record this pointer
	glfwSetWindowUserPointer(window, this);

	//register callback-function when window resized
	glfwSetWindowSizeCallback(window, resize);

	//register process when mouse wheel is operated
	glfwSetScrollCallback(window, wheel);

	//register process when keyboard is operated
	glfwSetKeyCallback(window, keyboard);



	//initialize opened window
	resize(window, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

int Window::shouldClose() const
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}
//swap color-buffers and get events 
void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}
void Window::pollEvents()
{
	glfwPollEvents();
}
bool Window::keyOn(int glfw_keyID)const
{
	return glfwGetKey(window, glfw_keyID) != GLFW_RELEASE;
}
bool Window::keyTriggered(int glfw_keyID)const
{
	return glfwGetKey(window, glfw_keyID) == GLFW_PRESS;
}
bool Window::buttonOn(int glfw_mouseButtonID) const
{
	return false;
}
void Window::resize(GLFWwindow * const window, int width, int height)
{
	glViewport(0, 0, width, height);

	//get this pointer
	Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

	if (instance != nullptr)
	{
		instance->size[0] = static_cast<GLfloat>(width);
		instance->size[1] = static_cast<GLfloat>(height);
	}
}

void Window::wheel(GLFWwindow * window, double x, double y)
{
	//get this pointer
	Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

	if (instance != nullptr)
	{
		instance->scale += static_cast<GLfloat>(y);
	}
}

void Window::keyboard(GLFWwindow * const window, int key, int scancode, int action, int mods)
{
	Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
	if (instance != nullptr)
	{
		instance->keyStatus = action;
	}
}
