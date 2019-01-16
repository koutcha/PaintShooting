#pragma once
#include <memory>
class GamePadXInput;
class Window;
class InputManager
{
public:
	InputManager(const std::shared_ptr<Window>& window,const std::shared_ptr<GamePadXInput>& joypad);
	~InputManager();
	bool keyOn(int glfw_keyID)const;
	const GamePadXInput& gamePad()const;
private:
	std::shared_ptr<Window> window;
	std::shared_ptr<GamePadXInput> joypad;

};

