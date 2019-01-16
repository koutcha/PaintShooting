#include "GameInfomation.h"
#include "Font.h"
#include "PaintBufferTexture.h"
#include "PaintColorSet.h"
#include "Window.h"
#include "GamingState.h"
#include "GameTimer.h"
#include "TextureRenderer2D.h"
#include"ResultAnimation.h"
#include <string>


GameInfomation::GameInfomation(int targetNumber, const std::shared_ptr<GameTimer>& timer, const std::shared_ptr<PaintColorSet>& color, const std::shared_ptr<PaintBufferTexture>& buffer, PLAYERNUM player):
	resultAnime(std::make_unique<ResultAnimation>()),
	color(color),
	buffer(buffer),
	timer(timer),
	lifeDataArray(targetNumber),
	teamIndices(targetNumber),
	player(player)
{
	percentage.fill(0.0f);
	timeAtCalculation.fill(0);
}

GameInfomation::~GameInfomation()
{
}

void GameInfomation::updatePaintPercentage()
{
	std::array<int,4> pixcelData = buffer->getPixcelData();

	float pixcelSum = buffer->getBufferResolution();
	pixcelSum *= pixcelSum;
	//濡れない部分のピクセルを引く
	pixcelSum -= pixcelData[2];
	//redPixcel
	percentage[0] = pixcelData[0] / pixcelSum;
	//greenPixcel
	percentage[1] = pixcelData[1] / pixcelSum;
	
	timeAtCalculation = calcClock(timer->getErapsedMinute());

}

void GameInfomation::sendCurrentColorIndex(int playerID, int teamIndex)
{
	if (playerID<0 || playerID>teamIndices.size())
	{
		return;
	}

	teamIndices[playerID] = teamIndex;
}

void GameInfomation::sendtLifeData(int playerID, float lifeData)
{
	
	if (playerID<0 || playerID>= lifeDataArray.size())
	{
		return;
	}
	
	lifeDataArray[playerID] = lifeData;
}

void GameInfomation::update(float dt, const GamingState state)
{
	if (state == GamingState::VS_RESULT && this->state == GamingState::VS_GAMESET)
	{
		updatePaintPercentage();
		resultAnime->startAnimation(percentage, 1.0f);
	}
	this->state = state;

	switch (state)
	{
	case GamingState::VS_RESULT:
		resultAnime->update(dt);
		break;
	default:
		break;
	}

}
void GameInfomation::draw(const Font & font, const TextureRenderer2D& tex,const Window & window)
{
	
	switch (state)
	{
	case GamingState::VS_PREPARE:
		drawPrepare(font, window);
		break;
	case GamingState::VS_PLAYING:
		drawVS(font, window);
		break;
	case GamingState::VS_GAMESET:
		drawGameSet(font, window);
		break;
	case GamingState::VS_RESULT:
		drawResult(font, window);
		break;
	case GamingState::TEST_PLAYING:
		drawTestMode(font, tex,window);
		break;
	default:
		break;
	}

}

void GameInfomation::drawPrepare(const Font & font, const Window & window)
{
	if (timer->getLimitTime() < 3.0f)
	{
		font.renderText("Ready...", window.getSize()[0] / 2.0f - 150.0f, window.getSize()[1] / 2.0f + 150.0f, 2.5f, Vector3f(1.0f, 1.0f, 1.0f), window);
		font.renderText(std::to_string(static_cast<int>(timer->getLimitTime()) + 1), window.getSize()[0] / 2.0f - 50.0f, window.getSize()[1] / 2.0f - 50.0f, 3.0f, Vector3f(1.0f, 1.0f, 1.0f), window);
	}
}

void GameInfomation::drawVS(const Font & font, const Window & window)
{
	const auto limitClock = calcClock(timer->getLimitTime());

	std::string clockText = std::to_string(limitClock[0]) + "m" + std::to_string(limitClock[1]) + "s";
	float clockScale = 1.5;
	font.renderText(clockText, window.getSize()[0] / 2.0f - font.getTextWidth(clockText,clockScale)/2.0f, window.getSize()[1] - 100.0f,clockScale, Vector3f(1.0f, 1.0f, 1.0f), window);
	font.renderText("Life:" + std::to_string(static_cast<int>(lifeDataArray[0])), 0.0f, 50.0f, 1.0f, color->getColor(teamIndices[0]), window);
	
	if (player == PLAYERNUM::TWO_PLAYER)
	{
		font.renderText("Life:" + std::to_string(static_cast<int>(lifeDataArray[1])), window.getSize()[0] / 2.0f, 50.0f, 1.0f, color->getColor(teamIndices[1]), window);
	}
	
}

void GameInfomation::drawGameSet(const Font & font, const Window & window)
{
	std::string gameSetMessage("GameSet!");
	float messageScale = 3.0f;
	font.renderText(gameSetMessage, window.getSize()[0] / 2.0f - font.getTextWidth(gameSetMessage, messageScale) / 2.0f, window.getSize()[1] / 2.0f, messageScale, Vector3f(1.0f, 1.0f, 1.0f), window);

}

void GameInfomation::drawResult(const Font & font, const Window & window)
{

	resultAnime->draw(font, window,*color);
}

void GameInfomation::drawTestMode(const Font & font,const TextureRenderer2D& render, const Window & window)
{
	const auto currentClock = calcClock(timer->getErapsedMinute());
	//塗り
	font.renderText(percentageString(percentage[0]) + "%", 0, window.getSize()[1] - 50.0f, 1.0f, color->getColor(0), window);
	font.renderText(percentageString(percentage[1]) + "%", 0, window.getSize()[1] - 100.0f, 1.0f, color->getColor(1), window);
	font.renderText("(at" + std::to_string(timeAtCalculation[0]) + "m" + std::to_string(timeAtCalculation[1]) + "s)", 150.0f, window.getSize()[1] - 75.0f, 1.0f, Vector3f(1.0f, 1.0f, 1.0f), window);


	//BulletColor
	font.renderText("BulletColor" + std::to_string(teamIndices[0]), 0.0f, window.getSize()[1] - 200.0f, 1.0f, color->getColor(teamIndices[0]), window);
	

	//現在時刻
	std::string clockText = std::to_string(currentClock[0]) + "m" + std::to_string(currentClock[1]) + "s";
	float clockScale = 1.5;
	font.renderText(clockText, window.getSize()[0] / 2.0f - font.getTextWidth(clockText, clockScale) / 2.0f, window.getSize()[1] - 100.0f, clockScale, Vector3f(1.0f, 1.0f, 1.0f), window);

	//塗り情報テクスチャ
	float texSize = 300.0f;
	buffer->bind(0);
	render.renderTextureNotBlending(0, 0, 0,texSize,texSize, window.getSize()[0], window.getSize()[1]);
	buffer->unbind(0);
	//操作説明補助
	font.renderText("Q Key or Srart: update percentage",texSize, 10.0f, 0.6f, Vector3f(1.0f, 1.0f, 1.0f), window);
	font.renderText("E Key or B-botton: change Bullet Color", texSize, 40.0f, 0.6f, Vector3f(1.0f, 1.0f, 1.0f), window);
}

std::array<int, 2> GameInfomation::calcClock(float seconds)
{
	std::array<int, 2> clock;
	clock[0] = seconds / 60.0f;
	clock[1] = seconds - clock[0]*60.0f;
	return clock;
}

std::string GameInfomation::percentageString(float percentage)
{
	int before = percentage*100.0f;
	int after = (percentage*100.0f - before)*100.0f;
	return std::to_string(before) + "." + std::to_string(after);
}
