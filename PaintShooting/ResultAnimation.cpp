#include "ResultAnimation.h"
#include "Window.h"
#include "Font.h"
#include "PaintColorSet.h"

ResultAnimation::ResultAnimation()
{
}


ResultAnimation::~ResultAnimation()
{
}

void ResultAnimation::startAnimation(const std::array<float, 2>& percentage,float timeDisplayWinTeam)
{
	time = 0;
	this->percentage = percentage;
	animatePercentageValue.fill(0.0f);
	winner = percentage[0] > percentage[1] ? 0 : 1;
	timeDisplayWinMessage = timeDisplayWinTeam;
}

void ResultAnimation::update(float dt)
{
	time += dt;
	if (time  >= timeDisplayWinMessage)
	{
		animatePercentageValue = percentage;
	}
	else
	{
		for (size_t i = 0; i < 2; ++i)
		{
			animatePercentageValue[i] = percentageLine(time, 0, timeDisplayWinMessage, 0.90f, 0, percentage[i], 0.2f);
		}

	}

}

void ResultAnimation::draw(const Font & font, const Window & window, const PaintColorSet & colorset)
{
	std::string percentMessage0(percentageString(animatePercentageValue[0]) + " %");
	std::string percentMessage1(percentageString(animatePercentageValue[1]) + " %");
	font.renderText(percentMessage0, window.getSize()[0] / 2.0f - window.getSize()[0] / 4.0f - font.getTextWidth(percentMessage0, 2.0f) / 2.0f, window.getSize()[1] / 2.0f, 2.0f, colorset.getColor(0), window);
	font.renderText(percentMessage1, window.getSize()[0] / 2.0f + window.getSize()[0] / 4.0f - font.getTextWidth(percentMessage1, 2.0f) / 2.0f, window.getSize()[1] / 2.0f, 2.0f, colorset.getColor(1), window);

	if (time >= timeDisplayWinMessage)
	{
		std::string winMessage("Team" + std::to_string(winner + 1) + " wins!");
		font.renderText(winMessage, window.getSize()[0] / 2.0f - font.getTextWidth(winMessage, 1.5f) / 2.0f, window.getSize()[1] / 2.0 - 200.0f, 1.5f, colorset.getColor(winner), window);

		font.renderText("Press 1 key or Push Back : Back to Title", 0.0f, 0.0f, 1.0f, Vector3f(1.0f, 1.0f, 1.0f), window);
	}


}
std::string ResultAnimation::percentageString(float percentage) const
{

	int before = percentage * 100.0f;
	int after = (percentage*100.0f - before)*100.0f;
	return std::to_string(before) + "." + std::to_string(after);

}

float ResultAnimation::percentageLine(float time,float start, float end, float changeRate, float min, float max, float valueRate) const
{
	float changeTime = (end - start)* changeRate +start;
	if (time < changeTime)
	{
		float nom = (max - min)*valueRate;
		float denom = (end - start)*changeRate;
		float k = nom / denom;
		float b = min - k * start;

		return k * time + b;
	}
	else
	{
		float nom = (max - min)*(1.0f - valueRate);
		float denom = (end - start)*(1.0f - changeRate);
		float k = nom / denom;
		float b = max - k * end;
		return k * time + b;
	}
	
}
