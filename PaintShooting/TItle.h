#pragma once
#include <memory>
#include "SequenceFrame.h"
class Font;
class TextureRenderer2D;
class MenuSystem;
class Texture;
class SoundManager;
class TitleBack;
class ColorGenerator;
class Title :
	public SequenceFrame
{
public:
	Title();
	~Title();

	virtual std::shared_ptr<SequenceFrame> update(const GameMain & game) override;
private:

	bool changeModeFlag;
	std::shared_ptr<Texture> cursurIcon;
	std::unique_ptr<MenuSystem> menu;
	std::unique_ptr<TextureRenderer2D> texRender;
	std::unique_ptr<TitleBack> back;
	std::shared_ptr<SoundManager> sounds;

};

