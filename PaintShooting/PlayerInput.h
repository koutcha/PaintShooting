#pragma once
#include "CharacterInput.h"
class PlayerInput:
	public CharacterInput
{
public:
	PlayerInput();
	explicit PlayerInput(int padID, float rotationValue);
	~PlayerInput();

	// CharacterInput ����Čp������܂���
	virtual void stopInput() override;
	virtual void update(const InputManager &) override;

	void keyInputMode(bool isEnable);
	void debugInput(bool isEnable);
	void setPadID(int padID);

private:
	//�΂ߓ��͕␳�l
	static const float OBLIQUE_CORRECT;
	//�L�[���͂𒲂ׂ�(enableKeyInput��false�Ȃ炷�ׂ�false)
	bool checkKeyInput(unsigned int keycode, const InputManager&);

	bool enableKeyInput;
	bool enableDebugInput;

	int padID;
	//�A�����͖h�~�p�̃t���O(���̂Ƃ���1�����Ȃ̂�bool�ŊǗ�)
	bool isInputChangeKeyBefore;

};

