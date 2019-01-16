#pragma once
namespace Sound
{
	//�T���v���i�X�e���I8bit,�X�e���I16bit

	struct FMTChank
	{
		unsigned char chunkID[4];
		unsigned long chunkSize;
		
		unsigned short formatID;
		unsigned short channelNum; //1 ���m�����@2 �X�e���I

		unsigned long samplingRate; //�T���v�����O���[�g
		unsigned long bytePerSec; //1�T���v��������̃r�b�g��
		unsigned short blockSize;
		unsigned short bitDepth;
	};
	struct  DataChank
	{
		unsigned char chunkID[4];
		unsigned long dataSize;
	};
	struct WaveHeader
	{
		unsigned char chunkID[4];
		unsigned long size8;
		unsigned char waveID[4];

		FMTChank fmt;
		DataChank data;
	};
	class Wave{
	public:
		Wave();
		virtual ~Wave();
		void printHeader()const;
		long length()const;
		double lenghtSecond()const;

		unsigned int formatID()const;
		unsigned int sampleingRate()const;
		unsigned int bitSize()const;
	protected:
		WaveHeader header;

	};


}
