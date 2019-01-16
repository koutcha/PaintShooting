#include "InputManager.h"
#include "Window.h"
#include "GamePadXInput.h"




InputManager::InputManager(const std::shared_ptr<Window>& window, const std::shared_ptr<GamePadXInput>& joypad):
	window(window),
	joypad(joypad)
{
}

InputManager::~InputManager()
{
}

bool InputManager::keyOn(int glfw_keyID) const
{
	return window->keyOn(glfw_keyID);
}

const GamePadXInput & InputManager::gamePad() const
{
	return *joypad;
}
