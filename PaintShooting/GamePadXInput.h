#pragma once

#pragma comment(lib,"xinput.lib ")

#include <windows.h>
#include <XInput.h>
#include <array>


class GamePadXInput
{
public:
	GamePadXInput();
	~GamePadXInput();
	GamePadXInput(const GamePadXInput&) = delete;
	void operator=(const GamePadXInput&) = delete;

	void updateState();
	bool checkConnection(int padID)const;
	//getState buttons and Dpad
	bool getButtonOn(int padID, unsigned short xInputButtonCode)const;
	short getLY(int padID)const;
	short getLX(int padID)const;
	short getRY(int padID)const;
	short getRX(int padID)const;

	unsigned char getRT(int padID)const;
	unsigned char getLT(int padID)const;
private:
	struct PadState
	{
		bool hasConnection;
		XINPUT_STATE state;
	};
	static constexpr int MAX_PAD_NUMBER = 4;
	std::array<PadState, MAX_PAD_NUMBER> padData;

};

