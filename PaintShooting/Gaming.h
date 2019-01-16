#pragma once
#include "SequenceFrame.h"
#include "Vector3.h"
#include <vector>
#include <array>
#include <memory>

//grphic
class GraphicIBL;
class IBLRenderPack;
class PerspectiveCamera;
class PointLights;

class SkyboxRender;
class TextureRenderer2D;

class Shape;
class MaterialPBR;

//charcter component
class PaintActor;
class PaintActorInput;
class CharacterInput;
class CharacterPhysics;
class CharacterAction;
class CharacterGraphics;
//weapon
class PaintWeapon;
//bomb
class Bomb;
class ActorCameraManager;

//stage
class Stage;
class StageGraphics;
class StagePhysics;


//paint sytem 
class PaintBufferTexture;
class PaintColorSet;

class PaintBulletManager;

class GameInfomation;
class GameTimer;

class SoundManager;

enum class GamingState;
enum class InitializeStateForGaming;

class Gaming :
	public SequenceFrame
{
public:
	Gaming(const InitializeStateForGaming& intState,const std::array<Vector3f,2>& colors);
	~Gaming();

	std::shared_ptr<SequenceFrame> update(const GameMain &main)override;
private:
	static const int ACTORNUMBER = 2;

	
	void updateInput(const GameMain &game);
	void updateGaming(const GameMain &game);
	void updateState(float dt);
	void	draw(const GameMain &game);
	void drawScene(const GameMain& game);

	GamingState state;

	std::shared_ptr<PerspectiveCamera> camera;
	std::shared_ptr<PointLights> lights;
	std::shared_ptr<IBLRenderPack> iblPack;
	std::shared_ptr<GraphicIBL> renderer;
	std::shared_ptr<TextureRenderer2D> texRender;
	std::shared_ptr<SkyboxRender> skybox;

	std::shared_ptr<Shape> box;
	std::shared_ptr<MaterialPBR> material;
	std::shared_ptr<PaintColorSet> colorset;
	
	//gameActor
	std::vector<std::shared_ptr<PaintActor>> actor;

	std::vector<std::shared_ptr<CharacterInput>> actorInput;
	std::vector<std::shared_ptr<CharacterPhysics>> actorPhysics;
	std::vector<std::shared_ptr<CharacterAction>> actorAction;
	std::vector<std::shared_ptr<CharacterGraphics>> actorGraphics;
	std::vector<std::shared_ptr<PaintWeapon>> weapons;
	std::vector<std::shared_ptr<Bomb>> bombs;


	std::shared_ptr<PaintBulletManager> bullets;

	std::shared_ptr<ActorCameraManager> actorCameraManager;
	
	std::unique_ptr<Stage> stage;
	std::shared_ptr<StageGraphics> stageGraphics;
	std::shared_ptr<StagePhysics> stagePhysics;
	std::shared_ptr<PaintBufferTexture> buffer;

	std::shared_ptr<GameInfomation> info;
	std::shared_ptr<GameTimer> timer;
	std::shared_ptr<SoundManager> sounds;

	bool draw2Player;
	int globalTimeCounter;
	float expo;

	float stateCounter;

	float gameSetTime;
	float gameTime;
	float prepareTime;
};

