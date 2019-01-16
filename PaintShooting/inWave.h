#pragma once
#include "Wave.h"
#include <iostream>

#include <fstream>
namespace Sound
{
	class InWave:public Wave
	{
	public:
		InWave();
		InWave(const char * file_path);
		~InWave();
		bool openFile(const char* filename);
		void closeFile();

		void getData(char* data);
		void getData(short*  data);

		void getData(char* Ldata, char* Rdata);
		void getData(short* Ldata, short* Rdata);

		void getDataBuffer(char* data, long head, long length);
		void getDataBuffer(short* data, long head, long length);

		void getDataBuffer(char* Ldata, char* Rdata, long head, long length);
		void getDataBuffer(short* Ldata, short* Rdata, long head, long length);

		
		bool hasData;
		bool isSuccessLoad()const;

	private:
		std::ifstream in;
		long dataBeg;
		bool isError;

	};
}



