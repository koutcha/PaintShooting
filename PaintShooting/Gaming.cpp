#define _USE_MATH_DEFINES
#include "Gaming.h"
#include "GameMain.h"
#include "Window.h"
#include "Vector3.h"

#include "PerspectiveCamera.h"
#include "PointLights.h"
#include "IBLRenderPack.h"
#include "GraphicIBL.h"
#include "MaterialPBR.h"
#include "SkyboxRender.h"
#include "Font.h"
#include "TextureRenderer2D.h"
#include "SolidShapeIndex.h"

#include "PaintActor.h"
#include "CharacterInput.h"
#include "PlayerInput.h"
#include "CPUInput.h"

#include "CharacterPhysics.h"
#include "CharacterGraphics.h"
#include "CharacterAction.h"

#include "ActorCameraManager.h"

#include "Stage.h"
#include "StageGraphics.h"
#include "StagePhysics.h"

#include "Matrix.h"

#include "InputManager.h"
#include "PaintColorSet.h"
#include "PaintWeapon.h"
#include "PaintBulletManager.h"

#include "PaintBufferTexture.h"

#include "MathAndPhysic.h"
#include "GameInfomation.h"
#include "GamingState.h"
#include "GameTimer.h"
#include "InitializeStateForGaming.h"
#include "SoundManager.h"
#include "GamePadXInput.h"
#include "PaintActorSetting.h"

#include "TItle.h"
#include "Bomb.h"

#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

Gaming::Gaming(const InitializeStateForGaming& iniState, const std::array<Vector3f, 2>& colors):
	camera(std::make_shared<PerspectiveCamera>(Vector3f(0.0f, 0.0f, 6.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0.3f, 2000.f, M_PI / 4.0f, 16.0f / 9.0f)),//カメラオブジェクトの作成
	lights(std::make_shared<PointLights>()),//ライトデータの作成
	iblPack(std::make_shared<IBLRenderPack>("Resource\\Image\\skybox.hdr")),//HDR画像からレンダリングに必要なデータを作成
	material(std::make_shared<MaterialPBR>(Vector3f(1.0, 1.0, 1.0), 0.4f, 0.4f, 1.0f)),//キャラクターマテリアルデータの作成
	colorset(std::make_shared<PaintColorSet>(colors[0],colors[1])),//ペイントのカラーセット（２色）の作成
	texRender(std::make_shared<TextureRenderer2D>()),//テクスチャレンダラーの作成
	skybox(std::make_shared<SkyboxRender>()),//スカイボックスのレンダラーを作成
	box(SolidShapeIndex::createBoxShape()),//メッシュデータの読み込み(四角形)
	actor(ACTORNUMBER),//キャラクターのコンポーネントのポインタ配列を作成
	actorInput(ACTORNUMBER),
	actorPhysics(ACTORNUMBER),
	actorAction(ACTORNUMBER),
	actorGraphics(ACTORNUMBER),
	weapons(ACTORNUMBER),
	bombs(ACTORNUMBER),
	draw2Player(true),
	globalTimeCounter(0),//フレームカウントの初期化
	expo(2.0),//あかるさ補正値の初期化
	state(GamingState::NONE),//ゲームステートを初期化
	prepareTime(3.0),//試合の待ち時間
	gameTime(0.0f),//試合時間
	gameSetTime(3.0),//試合後の待ち時間
	sounds(std::make_unique<SoundManager>(10))//サウンドマネージャ
{
	timer = std::make_shared<GameTimer>(sounds);

	

	switch (iniState)
	{
	case InitializeStateForGaming::TEST_MODE:
		state = GamingState::TEST_PLAYING;
		draw2Player = false;
		break;
	case InitializeStateForGaming::VS_MODE:
		state = GamingState::VS_PREPARE;
		draw2Player = true;
		sounds->dualPlayMode(true);
		timer->activateStartSignal(3);
		break;
	case InitializeStateForGaming::VS_CPU:
		state = GamingState::VS_PREPARE;
		timer->activateStartSignal(3);
		draw2Player = false;
		break;
	default:
		state = GamingState::TEST_PLAYING;
		draw2Player = true;
		break;
	}

	//create sound
	sounds->createBuffer("shot","Resource\\Sound\\gun.wav");
	sounds->createBuffer("death", "Resource\\Sound\\death.wav");
	sounds->createBuffer("bombShot", "Resource\\Sound\\bombShot.wav");
	sounds->createBuffer("bombExplode", "Resource\\Sound\\bombExplode.wav");
	sounds->createBuffer("count", "Resource\\Sound\\select07.wav");
	sounds->createBuffer("gameStart", "Resource\\Sound\\pickup01.wav");
	sounds->createBuffer("gameFinish", "Resource\\Sound\\select07.wav");

	timer->setCountSignalSoundKey("count");
	timer->setZeroSignaleSoundKey("gameStart");


	//プレイヤーの設定データ
	PaintActorSetting actorSetting;

	//ゲーム設定のデフォルト値
	gameTime = 90.0;
	//プレイヤーの共通設定のデフォルト値
	actorSetting.iniLife = 100;
	actorSetting.respownTime = 10.0f;
	actorSetting.velocity = 10.0f;
	actorSetting.angleDelta = 0.03f;

	std::ifstream reader("Setting\\GameSetting.json");
	//設定の読み込み
	if (reader)
	{
		json setting;
		reader >> setting;
		reader.close();

		try
		{
			//試合時間
			gameTime = setting["gametime"];
			//プレイ屋の設定
			actorSetting.iniLife = setting["player"]["life"];
			actorSetting.respownTime = setting["player"]["respownSecond"];
			actorSetting.velocity = setting["player"]["velocity"];
			actorSetting.angleDelta = setting["player"]["angleDelta"];
		}
		catch(json::type_error &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}


	//レンダラーの初期化
	renderer = std::make_unique<GraphicIBL>(camera, lights, iblPack);
	//ポイントライトのセッティング
	lights->setData(0, PointLightData(Vector3f(-30.0f, 30.0f, 0.0f), Vector3f(1.0, 1.0, 0.0), 1000.0f));
	lights->setData(1, PointLightData(Vector3f(0.0, 30.0, -30.0), Vector3f(1.0, 1.0, 0.0), 1000.0));
	lights->setData(2, PointLightData(Vector3f(30.0, 30.0, 0.0), Vector3f(1.0, 1.0, 0.0), 1000.0));
	lights->setData(3, PointLightData(Vector3f(0, 30.0, 30.0), Vector3f(1.0, 1.0, 0.0), 1000.0));;
	
	//ステージの初期化
	stagePhysics = std::make_shared<StagePhysics>();
	stageGraphics = std::make_shared<StageGraphics>(box);
	//ステージ半サイズ
	float stageScale = 40.0f;
	//ペイントバッファの作成
	buffer = std::make_shared<PaintBufferTexture>(stageScale);
	stage = std::make_unique<Stage>(stageScale, stagePhysics, stageGraphics,buffer,colorset);
	//弾システムの初期化
	int bulletMax = 300;
	bullets = std::make_shared<PaintBulletManager>(bulletMax, colorset);
	//ゲーム開始までの時間
	timer->startLimitCount(prepareTime);

	//UIの設定、初期化
	int infomationTargetNumber = 2;
	if (draw2Player)
	{
		info = std::make_shared<GameInfomation>(infomationTargetNumber,timer ,colorset, buffer,GameInfomation::PLAYERNUM::TWO_PLAYER);
	}
	else
	{
		info = std::make_shared<GameInfomation>(infomationTargetNumber,timer,colorset, buffer, GameInfomation::PLAYERNUM::ONE_PLAYER);
	}

	//カメラの設定,初期化
	float lengthFromTarget = 7.0;
	float heightFromTarget = 2.0;
	actorCameraManager = std::make_shared<ActorCameraManager>(camera, lengthFromTarget, heightFromTarget);

	
	WeaponSettingData data = PaintWeapon::createSettingFromJson("Setting\\Weapon.json");

	//ボムの設定
	BombSetting setting = Bomb::createSettingJson("Setting\\BombSetting.json");



	auto createCharacter = [&](int index,const std::shared_ptr<CharacterInput>& input)
	{
		
		actorInput[index] = input;
		actorPhysics[index] = std::make_shared<CharacterPhysics>();
		actorAction[index] = std::make_shared<CharacterAction>(sounds);
		actorGraphics[index] = std::make_shared<CharacterGraphics>(box, material, colorset);
		//武器の初期化
		weapons[index] = std::make_shared<PaintWeapon>(bullets, sounds, data);


		bombs[index] = std::make_shared<Bomb>(setting, box, colorset, material, bullets,sounds);
		actor[index] = std::make_shared<PaintActor>(actorSetting, actorInput[index], actorPhysics[index], actorAction[index], actorGraphics[index], weapons[index], bombs[index], bullets);

		actorAction[index]->setGameInfomation(info);
	};

	//1Pの個別設定
	actorSetting.playerID = 0;
	actorSetting.teamID = 0;
	actorSetting.padID = 0;
	actorSetting.iniPos = Vector3f(0.0f, 10.0f, 25.0f);
	actorSetting.iniAngle = M_PI;

	//キャラクターの初期化
	//その１
	auto playerInput0 = std::make_shared<PlayerInput>(actorSetting.padID, actorSetting.angleDelta);
	//1Pのみキーボード入力有効化
	playerInput0->keyInputMode(true);
	if (state == GamingState::TEST_PLAYING)
	{
		playerInput0->debugInput(true);
	}

	createCharacter(0, playerInput0);

	//2Pの個別設定
	actorSetting.playerID = 1;
	actorSetting.teamID = 1;
	actorSetting.padID = 1;
	actorSetting.iniPos = Vector3f(0, 10.0, -25);
	actorSetting.iniAngle = 0;

	//その2
	if (iniState == InitializeStateForGaming::VS_CPU)
	{
		auto cpuInput = std::make_shared<CPUInput>(30.0);
		cpuInput->addEnemy(actor[0]);
		createCharacter(1, cpuInput);

	
	}
	else
	{
		auto playerInput1 = std::make_shared<PlayerInput>(actorSetting.padID, actorSetting.angleDelta);
		if (state == GamingState::TEST_PLAYING)
		{
			playerInput0->debugInput(true);
		}

		createCharacter(1, playerInput1);

	}
	
	
	
}


Gaming::~Gaming()
{
	
}

std::shared_ptr<SequenceFrame> Gaming::update(const GameMain &main)
{
	updateInput(main);
	updateGaming(main);
	draw(main);
	updateState(main.getFrametime());
	globalTimeCounter++;
	if (state == GamingState::MOVE_TITLE)
	{
		return std::make_shared<Title>();
	}
	return nullptr;
}


void Gaming::updateInput(const GameMain & game)
{
	//windowオブジェクトのショートカット
	const Window& window = game.getWindow();
	//入力があれば塗りのパーセンテージ情報の更新


	if (state == GamingState::TEST_PLAYING || state == GamingState::VS_PLAYING)
	{
		//入力
		for (auto& inputs : actorInput)
		{
			inputs->update(game.getInput());
		}

		if (state == GamingState::TEST_PLAYING)
		{
			//入力があれば塗りのパーセンテージ情報の更新
			if (window.keyOn(GLFW_KEY_Q) || game.getInput().gamePad().getButtonOn(0, XINPUT_GAMEPAD_START))
			{
				info->updatePaintPercentage();
			}
		}
	}
	else if(state == GamingState::VS_PREPARE || state == GamingState::VS_RESULT)
	{
		//入力
		for (auto& inputs : actorInput)
		{
			inputs->stopInput();
		}
	}
	else if (state == GamingState::VS_GAMESET)
	{
		//入力
		for (auto& inputs : actorInput)
		{
			inputs->stopInput();
		}
	}

	if (state == GamingState::TEST_PLAYING || state == GamingState::VS_RESULT)
	{
		//タイトルに戻る
		if (window.keyOn(GLFW_KEY_1) || game.getInput().gamePad().getButtonOn(0, XINPUT_GAMEPAD_BACK))
		{
			state = GamingState::MOVE_TITLE;
		}
	}

}

void Gaming::updateGaming(const GameMain & main)
{
	float gravity = 9.8;
	//windowオブジェクトのショートカット
	const Window& window = main.getWindow();
	//位置情報の更新
	for (auto& physics : actorPhysics)
	{
		physics->update(main.getFrametime(), gravity);
	}
	for (auto& bomb : bombs)
	{
		bomb->update(main.getFrametime(),gravity);
	}
	bullets->update(main.getFrametime(),gravity);


	//キャラクターとステージの当たり判定
	for (auto& physics : actorPhysics)
	{
		physics->collision(*stagePhysics);
	}
	//ボムとステージ
	for (auto& bomb : bombs)
	{
		bomb->collision(*stagePhysics);
	}

	//弾とステージ
	bullets->collision(*stagePhysics);
	//キャラクターとボム、キャラクターと玉
	for (auto& physics : actorPhysics)
	{
		bullets->collision(*physics);
		for (auto& bomb : bombs)
		{
			physics->collision(*bomb);
		}
	}

	//行動データの更新(弾の生成、リスポーン判定など)
	for (auto& action : actorAction)
	{
		action->update(main.getFrametime());
	}
	//ブキの状態を更新
	for (auto& weapon : weapons)
	{
		weapon->update();
	}




	//時計の更新
	timer->update(main.getFrametime());
	//UIの更新
	info->update(main.getFrametime(),state);

	//塗り入力情報をテクスチャにレンダリング
	stageGraphics->renderInput();
}

void Gaming::updateState(float dt)
{

	if (state == GamingState::VS_PREPARE)
	{
		if (timer->getLimitTime() == 0.0f)
		{
		
			state = GamingState::VS_PLAYING;
			timer->startLimitCount(gameTime);
			timer->activateStartSignal(3);
			timer->setZeroSignaleSoundKey("gameFinish");

		}
	}
	else if (state == GamingState::VS_PLAYING)
	{

		if (timer->getLimitTime() == 0.0f)
		{
	
			state = GamingState::VS_GAMESET;
			gameSetTime = 4.0f;
			timer->startLimitCount(gameSetTime);

		}
	}
	else if (state == GamingState::VS_GAMESET)
	{

		if (timer->getLimitTime() == 0.0f)
		{

			state = GamingState::VS_RESULT;
			info->updatePaintPercentage();
		}
	}

	
}

void Gaming::draw(const GameMain & game)
{
	

	const Window& window = game.getWindow();
	if (draw2Player)
	{
		//カメラの更新
		camera->setAspect((window.getSize()[0]/2.0f) / window.getSize()[1]);
		glViewport(0, 0, window.getSize()[0] / 2.0f, window.getSize()[1]);
		//1P画面のレンダリング
		actorCameraManager->update(*actor[0]);
		drawScene(game);
		glViewport(window.getSize()[0] / 2.0f, 0, window.getSize()[0] / 2.0f, window.getSize()[1]);
		//2P画面のレンダリング
		actorCameraManager->update(*actor[1]);
		drawScene(game);
		//UIのレンダリング
		glViewport(0, 0, window.getSize()[0], window.getSize()[1]);
		info->draw(game.getFont(),*texRender, window);
		//Vector3f face = actor[0]->getPosition() - camera->getPosition();
		//sounds->setListenerInfo(camera->getPosition(), Vector3f(0, 1, 0), face);
		sounds->setMainListenerInfo(actor[0]->getPosition(), Vector3f(0, 1, 0), actor[0]->faceDirecion());
		sounds->setSubListenerInfo(actor[1]->getPosition(), Vector3f(0, 1, 0), actor[1]->faceDirecion());
		sounds->play();
	}
	else
	{
		//カメラの更新
		camera->setAspect(window.getSize()[0] / window.getSize()[1]);
		//カメラの更新
		glViewport(0, 0, window.getSize()[0], window.getSize()[1]);
		//メイン画面のレンダリング
		actorCameraManager->update(*actor[0]);
		drawScene(game);
		//UIのレンダリング
		info->draw(game.getFont(),*texRender,window);
		//Vector3f face = actor[0]->getPosition() - camera->getPosition();
		sounds->setMainListenerInfo(actor[0]->getPosition(), Vector3f(0, 1, 0),actor[0]->faceDirecion());
		//sounds->setListenerInfo(Vector3f(0, 0, 0), Vector3f(0, -1, 0), Vector3f(0, 0, 1));
		
		sounds->play();
	}
	

}

void Gaming::drawScene(const GameMain & game)
{
	const Window& window = game.getWindow();
	//レンダリング
	//キャラ
	for (auto& graphics : actorGraphics)
	{
		graphics->draw(*renderer);
	}
	//弾
	bullets->draw(*renderer);
	//ボム
	for(auto& bomb : bombs)
	{
		bomb->draw(*renderer);
	}

	//ステージのレンダリング
	stageGraphics->draw(*renderer);

	////スカイボックスのレンダリング
	iblPack->bindEnvironment(0);
	skybox->setHDRExposure(expo);
	skybox->drawSkyBox(*camera, 0);
	iblPack->unbindEnvironment(0);

	
}


