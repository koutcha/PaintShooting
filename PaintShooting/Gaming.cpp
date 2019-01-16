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
	camera(std::make_shared<PerspectiveCamera>(Vector3f(0.0f, 0.0f, 6.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0.3f, 2000.f, M_PI / 4.0f, 16.0f / 9.0f)),//�J�����I�u�W�F�N�g�̍쐬
	lights(std::make_shared<PointLights>()),//���C�g�f�[�^�̍쐬
	iblPack(std::make_shared<IBLRenderPack>("Resource\\Image\\skybox.hdr")),//HDR�摜���烌���_�����O�ɕK�v�ȃf�[�^���쐬
	material(std::make_shared<MaterialPBR>(Vector3f(1.0, 1.0, 1.0), 0.4f, 0.4f, 1.0f)),//�L�����N�^�[�}�e���A���f�[�^�̍쐬
	colorset(std::make_shared<PaintColorSet>(colors[0],colors[1])),//�y�C���g�̃J���[�Z�b�g�i�Q�F�j�̍쐬
	texRender(std::make_shared<TextureRenderer2D>()),//�e�N�X�`�������_���[�̍쐬
	skybox(std::make_shared<SkyboxRender>()),//�X�J�C�{�b�N�X�̃����_���[���쐬
	box(SolidShapeIndex::createBoxShape()),//���b�V���f�[�^�̓ǂݍ���(�l�p�`)
	actor(ACTORNUMBER),//�L�����N�^�[�̃R���|�[�l���g�̃|�C���^�z����쐬
	actorInput(ACTORNUMBER),
	actorPhysics(ACTORNUMBER),
	actorAction(ACTORNUMBER),
	actorGraphics(ACTORNUMBER),
	weapons(ACTORNUMBER),
	bombs(ACTORNUMBER),
	draw2Player(true),
	globalTimeCounter(0),//�t���[���J�E���g�̏�����
	expo(2.0),//�����邳�␳�l�̏�����
	state(GamingState::NONE),//�Q�[���X�e�[�g��������
	prepareTime(3.0),//�����̑҂�����
	gameTime(0.0f),//��������
	gameSetTime(3.0),//������̑҂�����
	sounds(std::make_unique<SoundManager>(10))//�T�E���h�}�l�[�W��
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


	//�v���C���[�̐ݒ�f�[�^
	PaintActorSetting actorSetting;

	//�Q�[���ݒ�̃f�t�H���g�l
	gameTime = 90.0;
	//�v���C���[�̋��ʐݒ�̃f�t�H���g�l
	actorSetting.iniLife = 100;
	actorSetting.respownTime = 10.0f;
	actorSetting.velocity = 10.0f;
	actorSetting.angleDelta = 0.03f;

	std::ifstream reader("Setting\\GameSetting.json");
	//�ݒ�̓ǂݍ���
	if (reader)
	{
		json setting;
		reader >> setting;
		reader.close();

		try
		{
			//��������
			gameTime = setting["gametime"];
			//�v���C���̐ݒ�
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


	//�����_���[�̏�����
	renderer = std::make_unique<GraphicIBL>(camera, lights, iblPack);
	//�|�C���g���C�g�̃Z�b�e�B���O
	lights->setData(0, PointLightData(Vector3f(-30.0f, 30.0f, 0.0f), Vector3f(1.0, 1.0, 0.0), 1000.0f));
	lights->setData(1, PointLightData(Vector3f(0.0, 30.0, -30.0), Vector3f(1.0, 1.0, 0.0), 1000.0));
	lights->setData(2, PointLightData(Vector3f(30.0, 30.0, 0.0), Vector3f(1.0, 1.0, 0.0), 1000.0));
	lights->setData(3, PointLightData(Vector3f(0, 30.0, 30.0), Vector3f(1.0, 1.0, 0.0), 1000.0));;
	
	//�X�e�[�W�̏�����
	stagePhysics = std::make_shared<StagePhysics>();
	stageGraphics = std::make_shared<StageGraphics>(box);
	//�X�e�[�W���T�C�Y
	float stageScale = 40.0f;
	//�y�C���g�o�b�t�@�̍쐬
	buffer = std::make_shared<PaintBufferTexture>(stageScale);
	stage = std::make_unique<Stage>(stageScale, stagePhysics, stageGraphics,buffer,colorset);
	//�e�V�X�e���̏�����
	int bulletMax = 300;
	bullets = std::make_shared<PaintBulletManager>(bulletMax, colorset);
	//�Q�[���J�n�܂ł̎���
	timer->startLimitCount(prepareTime);

	//UI�̐ݒ�A������
	int infomationTargetNumber = 2;
	if (draw2Player)
	{
		info = std::make_shared<GameInfomation>(infomationTargetNumber,timer ,colorset, buffer,GameInfomation::PLAYERNUM::TWO_PLAYER);
	}
	else
	{
		info = std::make_shared<GameInfomation>(infomationTargetNumber,timer,colorset, buffer, GameInfomation::PLAYERNUM::ONE_PLAYER);
	}

	//�J�����̐ݒ�,������
	float lengthFromTarget = 7.0;
	float heightFromTarget = 2.0;
	actorCameraManager = std::make_shared<ActorCameraManager>(camera, lengthFromTarget, heightFromTarget);

	
	WeaponSettingData data = PaintWeapon::createSettingFromJson("Setting\\Weapon.json");

	//�{���̐ݒ�
	BombSetting setting = Bomb::createSettingJson("Setting\\BombSetting.json");



	auto createCharacter = [&](int index,const std::shared_ptr<CharacterInput>& input)
	{
		
		actorInput[index] = input;
		actorPhysics[index] = std::make_shared<CharacterPhysics>();
		actorAction[index] = std::make_shared<CharacterAction>(sounds);
		actorGraphics[index] = std::make_shared<CharacterGraphics>(box, material, colorset);
		//����̏�����
		weapons[index] = std::make_shared<PaintWeapon>(bullets, sounds, data);


		bombs[index] = std::make_shared<Bomb>(setting, box, colorset, material, bullets,sounds);
		actor[index] = std::make_shared<PaintActor>(actorSetting, actorInput[index], actorPhysics[index], actorAction[index], actorGraphics[index], weapons[index], bombs[index], bullets);

		actorAction[index]->setGameInfomation(info);
	};

	//1P�̌ʐݒ�
	actorSetting.playerID = 0;
	actorSetting.teamID = 0;
	actorSetting.padID = 0;
	actorSetting.iniPos = Vector3f(0.0f, 10.0f, 25.0f);
	actorSetting.iniAngle = M_PI;

	//�L�����N�^�[�̏�����
	//���̂P
	auto playerInput0 = std::make_shared<PlayerInput>(actorSetting.padID, actorSetting.angleDelta);
	//1P�̂݃L�[�{�[�h���͗L����
	playerInput0->keyInputMode(true);
	if (state == GamingState::TEST_PLAYING)
	{
		playerInput0->debugInput(true);
	}

	createCharacter(0, playerInput0);

	//2P�̌ʐݒ�
	actorSetting.playerID = 1;
	actorSetting.teamID = 1;
	actorSetting.padID = 1;
	actorSetting.iniPos = Vector3f(0, 10.0, -25);
	actorSetting.iniAngle = 0;

	//����2
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
	//window�I�u�W�F�N�g�̃V���[�g�J�b�g
	const Window& window = game.getWindow();
	//���͂�����Γh��̃p�[�Z���e�[�W���̍X�V


	if (state == GamingState::TEST_PLAYING || state == GamingState::VS_PLAYING)
	{
		//����
		for (auto& inputs : actorInput)
		{
			inputs->update(game.getInput());
		}

		if (state == GamingState::TEST_PLAYING)
		{
			//���͂�����Γh��̃p�[�Z���e�[�W���̍X�V
			if (window.keyOn(GLFW_KEY_Q) || game.getInput().gamePad().getButtonOn(0, XINPUT_GAMEPAD_START))
			{
				info->updatePaintPercentage();
			}
		}
	}
	else if(state == GamingState::VS_PREPARE || state == GamingState::VS_RESULT)
	{
		//����
		for (auto& inputs : actorInput)
		{
			inputs->stopInput();
		}
	}
	else if (state == GamingState::VS_GAMESET)
	{
		//����
		for (auto& inputs : actorInput)
		{
			inputs->stopInput();
		}
	}

	if (state == GamingState::TEST_PLAYING || state == GamingState::VS_RESULT)
	{
		//�^�C�g���ɖ߂�
		if (window.keyOn(GLFW_KEY_1) || game.getInput().gamePad().getButtonOn(0, XINPUT_GAMEPAD_BACK))
		{
			state = GamingState::MOVE_TITLE;
		}
	}

}

void Gaming::updateGaming(const GameMain & main)
{
	float gravity = 9.8;
	//window�I�u�W�F�N�g�̃V���[�g�J�b�g
	const Window& window = main.getWindow();
	//�ʒu���̍X�V
	for (auto& physics : actorPhysics)
	{
		physics->update(main.getFrametime(), gravity);
	}
	for (auto& bomb : bombs)
	{
		bomb->update(main.getFrametime(),gravity);
	}
	bullets->update(main.getFrametime(),gravity);


	//�L�����N�^�[�ƃX�e�[�W�̓����蔻��
	for (auto& physics : actorPhysics)
	{
		physics->collision(*stagePhysics);
	}
	//�{���ƃX�e�[�W
	for (auto& bomb : bombs)
	{
		bomb->collision(*stagePhysics);
	}

	//�e�ƃX�e�[�W
	bullets->collision(*stagePhysics);
	//�L�����N�^�[�ƃ{���A�L�����N�^�[�Ƌ�
	for (auto& physics : actorPhysics)
	{
		bullets->collision(*physics);
		for (auto& bomb : bombs)
		{
			physics->collision(*bomb);
		}
	}

	//�s���f�[�^�̍X�V(�e�̐����A���X�|�[������Ȃ�)
	for (auto& action : actorAction)
	{
		action->update(main.getFrametime());
	}
	//�u�L�̏�Ԃ��X�V
	for (auto& weapon : weapons)
	{
		weapon->update();
	}




	//���v�̍X�V
	timer->update(main.getFrametime());
	//UI�̍X�V
	info->update(main.getFrametime(),state);

	//�h����͏����e�N�X�`���Ƀ����_�����O
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
		//�J�����̍X�V
		camera->setAspect((window.getSize()[0]/2.0f) / window.getSize()[1]);
		glViewport(0, 0, window.getSize()[0] / 2.0f, window.getSize()[1]);
		//1P��ʂ̃����_�����O
		actorCameraManager->update(*actor[0]);
		drawScene(game);
		glViewport(window.getSize()[0] / 2.0f, 0, window.getSize()[0] / 2.0f, window.getSize()[1]);
		//2P��ʂ̃����_�����O
		actorCameraManager->update(*actor[1]);
		drawScene(game);
		//UI�̃����_�����O
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
		//�J�����̍X�V
		camera->setAspect(window.getSize()[0] / window.getSize()[1]);
		//�J�����̍X�V
		glViewport(0, 0, window.getSize()[0], window.getSize()[1]);
		//���C����ʂ̃����_�����O
		actorCameraManager->update(*actor[0]);
		drawScene(game);
		//UI�̃����_�����O
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
	//�����_�����O
	//�L����
	for (auto& graphics : actorGraphics)
	{
		graphics->draw(*renderer);
	}
	//�e
	bullets->draw(*renderer);
	//�{��
	for(auto& bomb : bombs)
	{
		bomb->draw(*renderer);
	}

	//�X�e�[�W�̃����_�����O
	stageGraphics->draw(*renderer);

	////�X�J�C�{�b�N�X�̃����_�����O
	iblPack->bindEnvironment(0);
	skybox->setHDRExposure(expo);
	skybox->drawSkyBox(*camera, 0);
	iblPack->unbindEnvironment(0);

	
}


