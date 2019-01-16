#pragma once
#include "CharacterInput.h"
class PlayerInput:
	public CharacterInput
{
public:
	PlayerInput();
	explicit PlayerInput(int padID, float rotationValue);
	~PlayerInput();

	// CharacterInput を介して継承されました
	virtual void stopInput() override;
	virtual void update(const InputManager &) override;

	void keyInputMode(bool isEnable);
	void debugInput(bool isEnable);
	void setPadID(int padID);

private:
	//斜め入力補正値
	static const float OBLIQUE_CORRECT;
	//キー入力を調べる(enableKeyInputがfalseならすべてfalse)
	bool checkKeyInput(unsigned int keycode, const InputManager&);

	bool enableKeyInput;
	bool enableDebugInput;

	int padID;
	//連続入力防止用のフラグ(今のところ1つだけなのでboolで管理)
	bool isInputChangeKeyBefore;

};

