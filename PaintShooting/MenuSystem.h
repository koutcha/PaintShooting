#pragma once
#include <vector>
#include <memory>
#include "Vector2.h"
class InputManager;
class FontTextString32;
class Font;
class Texture;
class TextureRenderer2D;
class SoundManager;
class Window;
class MenuSystem
{
public:
	MenuSystem(const std::shared_ptr<SoundManager>& sounds,const std::shared_ptr<Texture>& cursurIcon, std::initializer_list<std::u32string> menuString);
	~MenuSystem();
	void input(const InputManager&);
	unsigned int getChoice()const;
	void draw(const Font& font,const TextureRenderer2D&,const Window& window);
private:
	unsigned int currentChoice;
	std::vector<FontTextString32> menuText;
	std::shared_ptr<Texture> cursurIcon;
	std::shared_ptr<SoundManager> sounds;
	Vector2f cursurPosiiton;
	Vector2f cursurSize;

	int timer;
	int waitTime;
};

