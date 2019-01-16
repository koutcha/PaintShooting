#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Vector3.h"
namespace Sound
{
	class SoundFile;
	class SoundBuffer;
	class SoundSource;
}
class SubManagerFor3DAudio;
class SoundManager
{
public:
	explicit SoundManager(int sourceNumber);
	~SoundManager();

	void setMainListenerInfo(const Vector3f& position, const Vector3f& up, const Vector3f face);
	void setSubListenerInfo(const  Vector3f& position, const Vector3f& up, const Vector3f face);
	void dualPlayMode(bool isOn);
	//�����f�[�^�o�b�t�@�̍쐬
	void createBuffer(const std::string& key,const std::string& filepath);
	//�v�Z�ʂ������̂ł��܂�Ă΂Ȃ�����
	void destroyBuffer(const std::string& key);
	//���̉����𖳎����čĐ�
	void reservePlaying(const std::string& bufferKey);
	//���̉����ōĐ�
	void reservePlaying3D(const std::string& bufferKey,const Vector3f& position);


	//play SE
	void play();
private:
	struct Reserve
	{
		std::string key;
		bool isRelative;
		Vector3f position;
	};
	struct Listener
	{
		Vector3f position;
		Vector3f up;
		Vector3f face;
	};

	void setListenerInfo(const  Vector3f& position, const Vector3f& up, const Vector3f face,int index);

	std::unordered_map<std::string,std::unique_ptr<Sound::SoundBuffer>> buffers;
	std::vector<Sound::SoundSource> sources;
	std::vector<Sound::SoundSource> loop;


	std::vector<SubManagerFor3DAudio> subManagers;

	std::vector<Reserve> reserveStack;
	size_t stackCount;

	unsigned int nextUse;

	bool isDualPlayAudio;
};

