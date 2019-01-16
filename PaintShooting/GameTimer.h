#pragma once
#include <string>
#include <memory>
class SoundManager;
class GameTimer
{
public:
	GameTimer(const std::shared_ptr<SoundManager>& sound);
	~GameTimer();
	void update(float dt);
	void startLimitCount(float limitTime);
	float getErapsedMinute()const;
	float getLimitTime()const;
	//���~�b�g0�ɂȂ�ƃA�N�e�B�x�[�g���؂��,countTime���特����
	void activateStartSignal(int countTime);

	void setCountSignalSoundKey(const std::string& key);
	void setZeroSignaleSoundKey(const std::string& key);
private:
	float erapsedMinutes;
	float limitTime;
	int signalCountTime;

	bool isStartSignalActivated;
	std::string countSignnalSound;
	std::string zeroSignalSound;

	std::shared_ptr<SoundManager> sound;
};

