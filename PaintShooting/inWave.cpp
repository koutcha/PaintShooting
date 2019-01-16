#include "inWave.h"
#include <string>
#include <memory>
using namespace std;
namespace Sound
{
	

	InWave::InWave():
		hasData(false)
	{
	}

	InWave::InWave(const char* file_path):
		hasData(false),
		isError(false)
	{
		if (!openFile(file_path))
		{
			isError = true;
		}
	}

	InWave::~InWave()
	{
		closeFile();
	}

	bool InWave::openFile(const char * filename)
	{
		if (hasData)
		{
			std::cerr << "this has a file" << std::endl;
			return false;
		}
		in.open(filename,ios::binary);
		if (!in)
		{
			cerr << "file can't open" << endl;
			return false;
		}
		in.read((char*)&header, sizeof(WaveHeader));
		dataBeg = in.tellg();

		//フォーマットの検査(ヘッダーのビットを比較)
		if ((memcmp(&header.chunkID, "RIFF", 4) != 0)
			|| (memcmp(&header.waveID, "WAVE", 4) != 0)
			|| (header.fmt.formatID != 1)) {

			std::cerr << "THIS FILE IS NOT \"RIFF waveform Audio Format\"" << std::endl;
			in.close();
			return false;
		}

		if (memcmp(&header.data.chunkID, "data", 4) != 0) {
			char chunkIDTemp[4];
			long chunkSizeTemp;

			// 先頭から riffチャンク分(12) + fmtチャンク分(8+chunk_size)
			in.seekg(12 + 8 + header.fmt.chunkSize,ios::beg);
			//fseeko64(in, 12 + 8 + header.fmt.chunksize, SEEK_SET);

			// 見つかったチャンクをサイズを取得してから読み飛ばして次のチャンクのIDを取得
			// whileループを抜けた時点でdataチャンクにたどりついている。
			// その後ろのデータサイズを取得してdataチャンクへと代入
			in.read((char*)&chunkIDTemp, sizeof(long));
			//fread(chunk_ID_temp, 1, 4, in);
			while (memcmp(chunkIDTemp, "data", 4) != 0) {
				in.read((char*)&chunkSizeTemp, sizeof(long));
				//fread(&chunksize_temp, 4, 1, in);
				in.seekg(chunkSizeTemp, ios::cur);
				//fseeko64(in, chunksize_temp, SEEK_CUR);
				in.read((char*)&chunkIDTemp, sizeof(long));
				//fread(chunk_ID_temp, 1, 4, in);
			}
			in.read((char*)&chunkSizeTemp, sizeof(long));
			//fread(&chunksize_temp, 4, 1, in);

			dataBeg = in.tellg(); // データ位置確定

									 //dataチャンク更新
			for (int i = 0; i<4; i++) {
				header.data.chunkID[i] = chunkIDTemp[i];
			}
			header.data.dataSize = chunkSizeTemp;
		}

		hasData= true;
		return true;
		

	}

	void InWave::closeFile()
	{
		in.close();
		header = { 0 };
		hasData = false;
	}

	void InWave::getData(char * data)
	{
		getDataBuffer(data, 0, length());
	}

	void InWave::getData(short * data)
	{
		getDataBuffer(data, 0, length());

	}

	void InWave::getData(char * Ldata,char * Rdata)
	{
		getDataBuffer(Ldata,Rdata, 0, length());

	}

	void InWave::getData(short * Ldata,short * Rdata)
	{
		getDataBuffer(Ldata, Rdata, 0, length());

	}

	void InWave::getDataBuffer(char * data,  long head,long length)
	{
		in.seekg(dataBeg + head,ios::beg);

		// モノラルデータはそのまま，ステレオデータは全部読んでから右と左を足して2で割る。
		if (header.fmt.channelNum == 1) {
			in.read(data, sizeof(char)*length);
			//fread(data, 1, length, in);

		}
		else {
			cout << length *2 << endl;
			unique_ptr<char[]> data_temp(make_unique<char[]>(length * 2));
			in.read((char*)data_temp.get(), sizeof(char)*length * 2);
			//char *data_temp = new char[length * 2];
			//in.read(data, sizeof(char)*length * 2);
			//read(data_temp, 1, data_length * 2, in);
			for (unsigned int i = 0; i<length; i++) {
				// キャストは桁あふれ対策
				data[i] = (char)(((short)data_temp[2 * i] + (short)data_temp[2 * i + 1]) / 2);
			}
			//delete[] data_temp;
		}

	}

	void InWave::getDataBuffer(short * data, long head, long length)
	{
		in.seekg(dataBeg + head, ios::beg);

		// モノラルデータはそのまま，ステレオデータは全部読んでから右と左を足して2で割る。
		if (header.fmt.channelNum == 1) {
			//ok 動作確認済み
			in.read((char*)data, sizeof(short)*length);
			//fread(data, 1, length, in);
			cout << "woomy" << endl;
		}
		else {

			//cout << length *sizeof(short)* 2 << endl;
			unique_ptr<short[]> data_temp(make_unique<short[]>(length * 2));
			in.read((char*)data_temp.get(), sizeof(short)*length * 2);
			//short *data_temp = new short[length * 2];
			//in.read((char*)data_temp, sizeof(short)*length * 2);
			//read(data_temp, 1, data_length * 2, in);
			for (unsigned int i = 0; i<length; i++) {
				// キャストは桁あふれ対策
				data[i] = (short)(((long)data_temp[2 * i] + (long)data_temp[2 * i + 1]) / 2);
				//cout << data[i] << endl;
			}
			//delete[] data_temp;
		}

	}

	void InWave::getDataBuffer(char * Ldata,char * Rdata, long head, long length)
	{
		in.seekg(dataBeg + head, ios::beg);
		//fseeko64(in, data_beg + head, SEEK_SET);

		// ステレオデータはそのまま，モノラルデータは両方に同じ値
		if (header.fmt.channelNum == 1) {
			if (Ldata != nullptr) in.read(Ldata,sizeof(char)*length);
			if (Rdata != nullptr) in.read(Rdata,sizeof(char)*length);

		}
		else {
			char *data_temp = new char[length * 2];
			in.read(data_temp,sizeof(char)*length*2);
			//fread(data_temp, 1, data_length * 2, in);
			if (Ldata != nullptr) {
				for (long i = 0; i<length; i++) {
					Ldata[i] = data_temp[i * 2];
				}
			}
			if (Rdata != nullptr) {
				for (long i = 0; i<length; i++) {
					Rdata[i] = data_temp[i * 2 + 1];
				}
			}

			delete[] data_temp;
		}
	}

	void InWave::getDataBuffer(short* Ldata, short * Rdata, long head, long length)
	{
		in.seekg(dataBeg + head, ios::beg);
		//fseeko64(in, data_beg + head, SEEK_SET);

		// ステレオデータはそのまま，モノラルデータは両方に同じ値
		if (header.fmt.channelNum == 1) {
			if (Ldata != nullptr) in.read((char*)Ldata, sizeof(short)*length);
			if (Rdata != nullptr) in.read((char*)Rdata, sizeof(short)*length);

		}
		else {
			short *data_temp = new short[length * 2];
			in.read((char*)data_temp, sizeof(short)*length * 2);
			//fread(data_temp, 1, data_length * 2, in);
			if (Ldata != nullptr) {
				for (long i = 0; i<length; i++) {
					Ldata[i] = data_temp[i * 2];
				}
			}
			if (Rdata != nullptr) {
				for (long i = 0; i<length; i++) {
					Rdata[i] = data_temp[i * 2 + 1];
				}
			}

			delete[] data_temp;
		}
	}

	bool InWave::isSuccessLoad() const
	{
		return !isError;
	}

}

