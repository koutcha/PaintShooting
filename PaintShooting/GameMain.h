#pragma once
#include<memory>
class SequenceFrame;
class Window;
class InputManager;
class GamePadXInput;
class Font;
class GameMain
{
public:
	GameMain(float updateInterval,const  std::shared_ptr<Window>& window);
	~GameMain();
	void updateGame();
	void setFrameTime(float dt);
	float getFrametime()const;
	const Window& getWindow()const;
	const InputManager& getInput()const;
	const Font& getFont()const;
private:
	std::shared_ptr<SequenceFrame> currentSeq;
	std::shared_ptr<Window> window;
	std::shared_ptr<GamePadXInput> joypad;
	std::shared_ptr<InputManager> input;
	std::shared_ptr<Font> font;
	float frameTime;
};

