#pragma once
#include <array>
#include <string>
#include <vector>
class Font;
class Window;
class PaintColorSet;
class PaintBufferTexture;
class GameTimer;
class TextureRenderer2D;
class ResultAnimation;
enum class GamingState;
class GameInfomation
{
public:
	enum class PLAYERNUM
	{
		ONE_PLAYER,
		TWO_PLAYER
	};
	GameInfomation(int targetNumber,const std::shared_ptr<GameTimer>& timer,const std::shared_ptr<PaintColorSet>& color,const std::shared_ptr<PaintBufferTexture>& buffer,PLAYERNUM player);
	~GameInfomation();

	void updatePaintPercentage();

	void sendCurrentColorIndex(int playerID,int teamIndex);
	void sendtLifeData(int playerID,float lifeData);
	
	void update(float dt, const GamingState state);

	void draw(const Font& font,const TextureRenderer2D& tex,const Window& window);
	void drawPrepare(const Font& font, const Window& window);
	void drawVS(const Font& font, const Window& window);
	void drawGameSet(const Font& font, const Window& window);

	void drawResult(const Font& font, const Window& window);
	void drawTestMode(const Font& font,const TextureRenderer2D& tex, const Window& window);

private:
	PLAYERNUM player;
	GamingState state;
	static std::array<int, 2> calcClock(float seconds);
	static std::string percentageString(float percentage);

	std::unique_ptr<ResultAnimation> resultAnime;

	std::shared_ptr<GameTimer> timer;
	std::shared_ptr<PaintColorSet> color;

	std::shared_ptr<PaintBufferTexture> buffer;
	std::array<float, 2> percentage;
	std::array<int, 2> timeAtCalculation;

	std::vector<float> lifeDataArray;
	std::vector<int> teamIndices;
	
};

