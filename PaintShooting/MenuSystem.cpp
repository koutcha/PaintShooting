#include "MenuSystem.h"
#include "Vector2.h"
#include "FontTextString32.h"
#include "Vector3.h"
#include "Window.h"
#include "TextureRenderer2D.h"
#include "Texture.h"
#include "GLFW/glfw3.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "GamePadXInput.h"
#include "Window.h"
#include <string>

MenuSystem::MenuSystem(const std::shared_ptr<SoundManager>& sounds,const std::shared_ptr<Texture>& cursurIcon, std::initializer_list<std::u32string> menulist):
	cursurIcon(cursurIcon),
	menuText(menulist.size()),
	waitTime(10),
	currentChoice(0),
	sounds(sounds)
{
	//èâä˙ê›íË
	int size = menulist.size();
	float heightInterval = 80.0f;
	float left = 100.0f;
	float bottom = 100.0f;

	float scale = 1.3f;

	Vector2f position(left, bottom + heightInterval * (menulist.size() - 1));
	Vector3f color(0.0f, 0.0f, 0.0f);
	
	int count = 0;
	for (auto i = menulist.begin(); i != menulist.end(); i++)
	{
		
		std::cout << i << std::endl;
		menuText[count].setText(*i);
		menuText[count].setPosition(position);
		menuText[count].setScale(scale);
		menuText[count].setColor(color);

		position.y -= heightInterval;
		count++;
	}

	
}

MenuSystem::~MenuSystem()
{
}

void MenuSystem::input(const InputManager &input)
{
	if (timer > 0)
	{
		timer--;
		return;
	}
	float deadZone = 4000;
	if (input.keyOn(GLFW_KEY_W) ||input.gamePad().getLY(0)>deadZone)
	{
		
		if(currentChoice == 0)
		{
			currentChoice = menuText.size() - 1;
		}
		else
		{
			currentChoice--;
		}
		timer = waitTime;
		sounds->reservePlaying("menu");
	}
	else if (input.keyOn(GLFW_KEY_S) || input.gamePad().getLY(0) < -deadZone)
	{
		currentChoice++;
		if (currentChoice > menuText.size() - 1)
		{
			currentChoice = 0;
		}
		timer = waitTime;
		sounds->reservePlaying("menu");
	}
		
}




unsigned int MenuSystem::getChoice() const
{
	return currentChoice;
}

void MenuSystem::draw(const Font & font, const TextureRenderer2D& renderer, const Window & window)
{
	int count = 0;
	for (auto& text : menuText)
	{
		text.draw(font,window.getSize()[0],window.getSize()[1]);
		if (count == currentChoice)
		{
			cursurIcon->bind(0);
			renderer.renderTexture(0, text.getPosition().x -60, text.getPosition().y, 50, 50, window.getSize()[0], window.getSize()[1]);
			cursurIcon->unbind(0);
		}
		count++;
	}
}
