#include "Audio.h"
#include <iostream>
using namespace std;
namespace Sound {
	Audio::Audio():
		device(alcOpenDevice(nullptr), [](ALCdevice* device) {  cout << "device del" << endl; return alcCloseDevice(device); }),
		context(alcCreateContext(device.get(), nullptr), [](ALCcontext* context) {cout << "context del" << endl; return alcDestroyContext(context);  })
	{
		alcMakeContextCurrent(context.get());
		cout << "launch audio" << endl;
	}


	Audio::~Audio()
	{
		
		alcMakeContextCurrent(nullptr);

	}
}


