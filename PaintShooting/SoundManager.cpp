#include "SoundManager.h"
#include "AL\al.h"
#include "AL\alc.h"

#include "SoundFile.h"
#include "SoundSource.h"
#include "SoundBuffer.h"
#include "SubManagerFor3DAudio.h"

#include <iostream>

SoundManager::SoundManager(int sourceNumber):
	sources(sourceNumber),
	reserveStack(sourceNumber),
	nextUse(0),
	stackCount(0),
	isDualPlayAudio(false)
{
	
	subManagers.reserve(2);
	subManagers.emplace_back(sourceNumber);
	subManagers.emplace_back(sourceNumber);

}

SoundManager::~SoundManager()
{
}

void SoundManager::setMainListenerInfo(const Vector3f & position, const Vector3f & up, const Vector3f face)
{
	setListenerInfo(position, up, face, 0);
}

void SoundManager::setSubListenerInfo(const Vector3f & position, const Vector3f & up, const Vector3f face)
{
	setListenerInfo(position, up, face, 1);
}

void SoundManager::dualPlayMode(bool isOn)
{
	isDualPlayAudio = isOn;
}

void SoundManager::createBuffer(const std::string & key, const std::string & filepath)
{
	Sound::SoundFile file(filepath.c_str());
	
	if (file.checkHasData())
	{
		auto itr = buffers.find(key);
		if (itr == buffers.end())
		{
			buffers.emplace(key, std::make_unique<Sound::SoundBuffer>(file));
		}
		else
		{
			std::cout << key << "is already register" << std::endl;
		}
		
	}
	else
	{
		std::cout << key << "no data" << std::endl;
	}
}

void SoundManager::destroyBuffer(const std::string & key)
{
	buffers.erase(key);
}



void SoundManager::reservePlaying(const std::string & bufferKey)
{
	if (stackCount >= reserveStack.size())
	{
		return;
	}

	reserveStack[stackCount].key = bufferKey;
	reserveStack[stackCount].isRelative = true;
	reserveStack[stackCount].position = Vector3f(0.0f,0.0f,0.0f);

	stackCount++;
}

void SoundManager::reservePlaying3D(const std::string & bufferKey, const Vector3f & position)
{
	if (stackCount >= reserveStack.size())
	{
		return;
	}

	reserveStack[stackCount].key = bufferKey;
	reserveStack[stackCount].isRelative = false;
	reserveStack[stackCount].position = position;

	stackCount++;
}


void SoundManager::play()
{
	if (isDualPlayAudio)
	{
	
		for (size_t i = 0; i < stackCount; ++i)
		{
			auto itr = buffers.find(reserveStack[i].key);
			if (itr == buffers.end())
			{
				continue;
			}


			if (reserveStack[i].isRelative)
			{
				subManagers[0].playRelative(*itr->second, reserveStack[i].position);
			}
			else
			{
				//¶‰æ–Ê
				Vector3f subPosition(-1.0f, 0.0f, 0.0f);
				subPosition *= (reserveStack[i].position - subManagers[0].getPosition()).length()+1;
				subManagers[0].playRelative(*itr->second, subPosition,2.0f);
				//‰E‰æ–Ê
				subPosition = Vector3f(1.0f, 0.0f, 0.0f);
				subPosition *= (reserveStack[i].position - subManagers[1].getPosition()).length()+1;
				subManagers[1].playRelative(*itr->second,subPosition,2.0f);
			}
		}

		stackCount = 0;
	}
	else
	{
		subManagers[0].bindListner();

		for (size_t i = 0; i < stackCount; ++i)
		{
			auto itr = buffers.find(reserveStack[i].key);
			if (itr == buffers.end())
			{
				continue;
			}


			if (reserveStack[i].isRelative)
			{
				subManagers[0].playRelative(*itr->second, reserveStack[i].position);
			}
			else
			{

				subManagers[0].play3D(*itr->second, reserveStack[i].position);
			}
		}

		stackCount = 0;
	}


}

void SoundManager::setListenerInfo(const Vector3f & position, const Vector3f & up, const Vector3f face, int index)
{
	subManagers[index].setPosition(position);
	subManagers[index].setUp(up);
	subManagers[index].setFace(face);
}

