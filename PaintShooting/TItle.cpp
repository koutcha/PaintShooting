#include "TItle.h"
#include "Font.h"
#include "MenuSystem.h"
#include "GameMain.h"
#include "Vector3.h"
#include "Texture.h"
#include "TextureRenderer2D.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "Gaming.h"
#include "InitializeStateForGaming.h"
#include "SoundManager.h"
#include "TitleBack.h"
#include "InputManager.h"
#include "GamePadXInput.h"
#include "ColorGenerator.h"
#include <string>
#include <array>
Title::Title():
	texRender(std::make_unique<TextureRenderer2D>()),
	cursurIcon(std::make_shared<Texture>("Resource\\Image\\point.png")),
	sounds(std::make_shared<SoundManager>(3)),
	changeModeFlag(false)
{
	ColorGenerator colorGen("Setting\\ColorSet.json");
	std::array<Vector3f, 2> color = colorGen.getRamdomColorSet();
	back = std::make_unique<TitleBack>(color[0], color[1]);
	sounds->createBuffer("decision", "Resource\\Sound\\decision.wav");
	sounds->createBuffer("menu", "Resource\\Sound\\menu.wav");

	menu.reset(new MenuSystem(sounds,cursurIcon, { U"テストモード",U"対戦",U"対戦(CPU)"}));
}



Title::~Title()
{
	std::cout << "del title" << std::endl;


}

std::shared_ptr<SequenceFrame> Title::update(const GameMain & game)
{
	if (changeModeFlag)
	{
		if (menu->getChoice() == 0)
		{
			return std::make_shared<Gaming>(InitializeStateForGaming::TEST_MODE, back->getColors());
		}
		else if (menu->getChoice() == 1)
		{
			return std::make_shared<Gaming>(InitializeStateForGaming::VS_MODE, back->getColors());
		}
		else if (menu->getChoice() == 2)
		{
			return std::make_shared<Gaming>(InitializeStateForGaming::VS_CPU, back->getColors());
		}
	}

	back->draw(*texRender, game.getWindow());
	game.getFont().renderText("Paint Shoot",100, 500,2.0 ,Vector3f(0.0f, 0.0f, 0.0f), game.getWindow());
	menu->input(game.getInput());
	menu->draw(game.getFont(),*texRender ,game.getWindow());


	if (game.getWindow().keyOn(GLFW_KEY_ENTER)|| game.getInput().gamePad().getButtonOn(0,XINPUT_GAMEPAD_A))
	{
		sounds->reservePlaying("decision");
		changeModeFlag = true;
		
	}

	sounds->play();

	return nullptr;
}
