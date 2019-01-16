#include "GamePadXInput.h"



GamePadXInput::GamePadXInput()
{
	updateState();
}


GamePadXInput::~GamePadXInput()
{
}

void GamePadXInput::updateState()
{

	DWORD dwResult;
	for (DWORD i = 0; i < MAX_PAD_NUMBER; i++)
	{

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &padData[i].state);

		if (dwResult == ERROR_SUCCESS)
		{
			padData[i].hasConnection = true;

			XINPUT_STATE& state = padData[i].state;
			if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				state.Gamepad.sThumbLX = 0;
				state.Gamepad.sThumbLY = 0;
			}

			if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				state.Gamepad.sThumbRX = 0;
				state.Gamepad.sThumbRY = 0;
			}


		}
		else
		{
			padData[i].hasConnection = false;;
		}
	}
}

bool GamePadXInput::checkConnection(int padID) const
{
	if (padID<0 || padID>=MAX_PAD_NUMBER)
	{
		return false;
	}
	return padData[padID].hasConnection;

}

bool GamePadXInput::getButtonOn(int padID, unsigned short xInputButtonCode) const
{
	if (padID < 0 || padID >= MAX_PAD_NUMBER)
	{
		return false;
	}
	if (padData[padID].hasConnection) {
		WORD buttons = padData[padID].state.Gamepad.wButtons;
		return  buttons & xInputButtonCode;
	}
	else
	{
		return false;
	}
}

short GamePadXInput::getLY(int padID) const
{
	if (checkConnection(padID))
	{
		return padData[padID].state.Gamepad.sThumbLY;

	}
	else
	{
		return 0;
	}
}

short GamePadXInput::getLX(int padID) const
{
	if (checkConnection(padID))
	{
		return padData[padID].state.Gamepad.sThumbLX;
	}
	else
	{
		return 0;
	}
}

short GamePadXInput::getRY(int padID) const
{
	if (checkConnection(padID))
	{
		return padData[padID].state.Gamepad.sThumbRY;

	}
	else
	{
		return 0;
	}
}

short GamePadXInput::getRX(int padID) const
{
	if (checkConnection(padID))
	{
		return padData[padID].state.Gamepad.sThumbRX;
	}
	else
	{
		return 0;
	}
}

unsigned char GamePadXInput::getRT(int padID) const
{
	if (checkConnection(padID))
	{
		return padData[padID].state.Gamepad.bRightTrigger;
	}
	else
	{
		return 0;
	}
}

unsigned char GamePadXInput::getLT(int padID) const 
{
	if (checkConnection(padID))
	{
		return padData[padID].state.Gamepad.bLeftTrigger;
	}
	else
	{
		return 0;
	}
}

