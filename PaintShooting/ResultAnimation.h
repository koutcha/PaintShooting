#pragma once
#include <array>
class Font;
class Window;
class PaintColorSet;
class ResultAnimation
{
public:
	ResultAnimation();
	~ResultAnimation();
	void startAnimation(const std::array<float, 2>& percentage,float timeDisplayWinTeam);
	void update(float dt);
	void draw(const Font& font, const Window& window, const PaintColorSet& colorset);
private:
	std::string percentageString(float percentage)const;
	float percentageLine(float time, float start, float end, float changeRate, float min, float max, float valueRate)const;
	float time;
	float timeDisplayWinMessage;
	std::array<float, 2> percentage;
	std::array<float, 2> animatePercentageValue;
	int winner;

};

