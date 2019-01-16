#include "GameMain.h"
#include "SequenceFrame.h"
#include "Gaming.h"
#include "TItle.h"
#include "InputManager.h"
#include "GamePadXInput.h"
#include "Font.h"
GameMain::GameMain(float updateInterval, const std::shared_ptr<Window>& window):
	frameTime(updateInterval),
	currentSeq(std::make_shared<Title>()),
	window(window),
	joypad(std::make_shared<GamePadXInput>()),
	font(std::make_shared<Font>("Resource\\Font\\test2.ttc", Font::getBasicCharSet() + Font::getKanaCharSet() + std::u32string(U"テストモード操作説明対戦CPU()")))
{
	input = std::make_shared<InputManager>(window,joypad);
}

GameMain::~GameMain()
{
}

void GameMain::updateGame()
{
	
	joypad->updateState();
	if (std::shared_ptr<SequenceFrame> temp = currentSeq->update(*this))
	{
		currentSeq = temp;
	}


}

void GameMain::setFrameTime(float dt)
{
	frameTime = dt;
}

float GameMain::getFrametime() const
{
	return frameTime;
}

const Window & GameMain::getWindow() const
{
	return *window;
}

const InputManager & GameMain::getInput() const
{
	return *input;
}

const Font & GameMain::getFont() const
{
	return *font;
}

