#pragma once
#include "AL\al.h"
#include "AL\alc.h"
#include "Vector3.h"
namespace Sound
{
	class SoundBuffer;
	class SoundSource
	{
	public:
		SoundSource();
		~SoundSource();
		void play(const SoundBuffer& buffer)const;
		//play 3D audio on World Coord
		void play3D(const SoundBuffer& buffer,const Vector3f& position)const;
		void play3D(const SoundBuffer& buffer, const Vector3f& position,float gain)const;

		//play 3D audio Relative Coord From Listener
		void playRelative(const SoundBuffer& buffer, const Vector3f& relativePosition)const;
		void playRelative(const SoundBuffer& buffer, const Vector3f& relativePosition,float gain)const;


		void playLoop(const SoundBuffer& buffer);
		void stopLoop(const SoundBuffer& buffer);
	private:
		ALuint sourceID;
	};
}


