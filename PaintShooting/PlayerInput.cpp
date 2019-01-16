#define _USE_MATH_DEFINES
#include "PlayerInput.h"
#include "CharacterInput.h"
#include "GL/glew.h"
#include"GLFW/glfw3.h"
#include "Window.h"
#include "RigidData.h"
#include "PaintActorData.h"
#include "InputManager.h"
#include "GamePadXInput.h"
const float PlayerInput::OBLIQUE_CORRECT = std::sqrt(2.0f) / 2.0f;

PlayerInput::PlayerInput()
{
}

PlayerInput::PlayerInput(int padID, float rotationValue):
	CharacterInput(rotationValue),
	padID(padID),
	enableKeyInput(false),
	enableDebugInput(false),
	isInputChangeKeyBefore(false)
{
}


PlayerInput::~PlayerInput()
{
}

void PlayerInput::stopInput()
{
	auto& data = getActorData();
	data->setAttackState(PaintActorAttackState::NEUTRAL);
	data->setMotionState(PaintActorMotionState::NEUTRAL);
	data->setFrontMove(0);
	data->setSideMove(0);
}

void PlayerInput::update(const InputManager &inputManager)
{
	auto& data = getActorData();
	auto& rigid = getActorRigid();
	//���͏�Ԃ̏�����
	data->setFrontMove(0);
	data->setSideMove(0);
	data->setAttackState(PaintActorAttackState::NEUTRAL);
	data->setMotionState(PaintActorMotionState::NEUTRAL);

	const GamePadXInput& pad = inputManager.gamePad();

	//���͏��(�U���{�^���A�W�����v�{�^��)
	PaintActorAttackState attackState = PaintActorAttackState::NEUTRAL;
	PaintActorMotionState actionState = PaintActorMotionState::NEUTRAL;

	//�O�����E�̓��͋��x
	float frontMove = 0.0;
	float sideMove = 0.0;

	float maxInputStrength = 1.0;

	//�J������1�t���[��������̉�]�p�x
	float angleVary = getRotationDelta();


	//�U��
	if (checkKeyInput(GLFW_KEY_F, inputManager) || pad.getRT(padID) > 0)
	{
		data->setAttackState(PaintActorAttackState::SHOOTING_PAINT_BULLET);
		//std::cout << "attack" << std::endl;
	}
	if (checkKeyInput(GLFW_KEY_G, inputManager) || pad.getButtonOn(padID, XINPUT_GAMEPAD_RIGHT_SHOULDER) )
	{
		data->setAttackState(PaintActorAttackState::SHOOTING_PAINT_BOMB);
	}

	//�W�����v
	if (checkKeyInput(GLFW_KEY_SPACE, inputManager) || pad.getButtonOn(padID, XINPUT_GAMEPAD_A))
	{
		data->setMotionState(PaintActorMotionState::JUMPING);
	}

	float deadzone = 4500;
	//�ړ�

	if (checkKeyInput(GLFW_KEY_W, inputManager) || pad.getLY(padID) > deadzone)
	{
		frontMove = maxInputStrength;
	}
	if (checkKeyInput(GLFW_KEY_S, inputManager) || pad.getLY(padID) < -deadzone)
	{
		frontMove -= maxInputStrength;
	}


	if (checkKeyInput(GLFW_KEY_A, inputManager) || pad.getLX(padID) < -deadzone)
	{
		sideMove -= maxInputStrength;

	}
	if (checkKeyInput(GLFW_KEY_D, inputManager) || pad.getLX(padID) > deadzone)
	{
		sideMove += maxInputStrength;

	}

	//�΂ߕ␳
	if (sideMove != 0.0f && frontMove != 0.0f)
	{
		sideMove *= OBLIQUE_CORRECT;
		frontMove *= OBLIQUE_CORRECT;
	}

	data->setFrontMove(frontMove);
	data->setSideMove(sideMove);

	//��]

	//���E y����]
	float yAngle = data->getAngleYaxis();
	//�㉺ x����]
	float xAngle = data->getAngleXaxis();

	//x���̉�]�ω���
	float xAngleValue = 0.0;
	//y���̉�]�ω���
	float yAngleValue = 0.0;

	if (checkKeyInput(GLFW_KEY_LEFT, inputManager) || pad.getRX(padID) < -deadzone)
	{
		yAngleValue = -angleVary;

	}
	if (checkKeyInput(GLFW_KEY_RIGHT, inputManager) || pad.getRX(padID) > deadzone)
	{
		yAngleValue = angleVary;

	}


	if (checkKeyInput(GLFW_KEY_UP, inputManager) || pad.getRY(padID) > deadzone)
	{
		xAngleValue = angleVary;

	}
	if (checkKeyInput(GLFW_KEY_DOWN, inputManager) || pad.getRY(padID) < -deadzone)
	{
		xAngleValue = -angleVary;

	}

	yAngle -= yAngleValue;
	xAngle += xAngleValue;

	//�l�̏C��
	if (yAngle > 2.0f* M_PI)
	{
		yAngle = 0;
	}
	if (yAngle < 0)
	{
		yAngle = 2.0f * M_PI;
	}


	//��]����
	if (xAngle > M_PI / 12.0f)
	{
		xAngle = M_PI / 12.0f;
	}
	if (xAngle < -M_PI / 6.0f)
	{
		xAngle = -M_PI / 6.0f;
	}

	data->setAngleYaxis(yAngle);
	data->setAngleXaxis(xAngle);


	//�e�X�g���[�h�Ȃ�F�̐؂�ւ���
	if (enableDebugInput && (checkKeyInput(GLFW_KEY_E, inputManager) || pad.getButtonOn(padID, XINPUT_GAMEPAD_B)))
	{
		//�F(�����`�[��)�̐؂�ւ�

		if (!isInputChangeKeyBefore)
		{
			data->setTeam((data->getTeam() + 1) % 2);
			isInputChangeKeyBefore = true;
		}
	}
	else
	{
		isInputChangeKeyBefore = false;
	}

}

void PlayerInput::keyInputMode(bool isEnable)
{
	enableKeyInput = isEnable;
}

void PlayerInput::debugInput(bool isEnable)
{
	enableDebugInput = isEnable;
}

void PlayerInput::setPadID(int padID)
{
	this->padID = padID;
}

bool PlayerInput::checkKeyInput(unsigned int keycode, const InputManager &input)
{
	return input.keyOn(keycode) && enableKeyInput;
}
