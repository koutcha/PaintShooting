#include "GameTimer.h"
#include "SoundManager.h"


GameTimer::GameTimer(const std::shared_ptr<SoundManager>& sound):
	sound(sound),
	limitTime(0.0f),
	erapsedMinutes(0.0f),
	signalCountTime(3),
	isStartSignalActivated(false)
{
}


GameTimer::~GameTimer()
{
}

void GameTimer::update(float dt)
{
	erapsedMinutes += dt;
	limitTime -= dt;
	
	if (isStartSignalActivated && limitTime < signalCountTime)
	{
		signalCountTime--;
		if (signalCountTime == -1)
		{
			isStartSignalActivated = false;
			sound->reservePlaying(zeroSignalSound);
		}
		else
		{
			sound->reservePlaying(countSignnalSound);
		}
		
	}
	if (limitTime < 0)
	{
		limitTime = 0.0f;

	}


}


void GameTimer::startLimitCount(float limitTime)
{
	this->limitTime = limitTime;
}

float GameTimer::getErapsedMinute() const
{
	return erapsedMinutes;
}

float GameTimer::getLimitTime() const
{
	return limitTime;
}

void GameTimer::activateStartSignal(int countTime)
{
	isStartSignalActivated = true;
	signalCountTime = countTime;
	
}

void GameTimer::setCountSignalSoundKey(const std::string & key)
{
	countSignnalSound = key;
}

void GameTimer::setZeroSignaleSoundKey(const std::string & key)
{
	zeroSignalSound = key;
}

