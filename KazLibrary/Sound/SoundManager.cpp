#include "SoundManager.h"
#include"../Helper/KazHelper.h"

SoundManager::SoundManager()
{
	//XAudioエンジンのインスタンスを生成
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスを生成
	xAudio2->CreateMasteringVoice(&masterVoice);
}

void SoundManager::Finalize()
{
	for (int i = 0; i < masterSoundData.size(); i++)
	{
		delete masterSoundData[i].soundData;
	}
}

short SoundManager::LoadSoundMem(string FILE_PASS, bool BGM_FLAG)
{
	//読み込み重複防止
	for (int i = 0; i < masterSoundData.size(); i++)
	{
		if (FILE_PASS == masterSoundData[i].filePass)
		{
			return i;
		}
	}


	ifstream file;
	file.open(FILE_PASS, ios_base::binary);
	if (file.fail())
	{
		FailCheck(FILE_PASS + "の読み込みに失敗しました\n");
		return -1;
	}

	//RIFF------------------------------------------
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		FailCheck(FILE_PASS + "の読み込みに失敗しました\n");
		return -1;
	}
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		FailCheck(FILE_PASS + "の読み込みに失敗しました\n");
		return -1;
	}
	//RIFF------------------------------------------


	//FormatChunk------------------------------------------
	FormatChunk format = {};
	file.read((char *)&format, sizeof(ChunkHeader));

	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		FailCheck(FILE_PASS + "の読み込みに失敗しました\n");
		return -1;
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char *)&format.fmt, format.chunk.size);
	//FormatChunk------------------------------------------

	//ChunkHeader------------------------------------------
	ChunkHeader data;
	file.read((char *)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}

	if (strncmp(data.id, "LIST ", 4) == 0)
	{
		file.seekg(data.size, ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		FailCheck(FILE_PASS + "の読み込みに失敗しました\n");
		return -1;
	}
	//ChunkHeader------------------------------------------


	//ファイル読み込み成功
	short tmp = handle.GetHandle();
	
	//true...配列以上ならpush,false...配列内ならそのまま置き換え
	if (masterSoundData.size() <= tmp)
	{
		//masterSoundData.push_back({});
		masterSoundData[tmp].soundData = new SoundData;
		masterSoundData[tmp].filePass = FILE_PASS;
	}
	else
	{
		masterSoundData[tmp].soundData = new SoundData;
		masterSoundData[tmp].filePass = FILE_PASS;
	}



	masterSoundData[tmp].soundData->wfex= format.fmt;
	masterSoundData[tmp].soundData->bufferSize= data.size;	
	masterSoundData[tmp].soundData->pBuffer = new BYTE[data.size];
	file.read(reinterpret_cast<char *>(masterSoundData[tmp].soundData->pBuffer), data.size);
	file.close();


	//サウンドソース生成
	if (BGM_FLAG)
	{
		xAudio2->CreateSourceVoice(&masterSoundData[tmp].soundSorce, &masterSoundData[tmp].soundData->wfex);
	}



	SucceedCheck(FILE_PASS + "の読み込みに成功しました\n");

	return tmp;
}

void SoundManager::PlaySoundMem(short HANDLE, int VOLUME, bool LOOP_FLAG)
{
	if (KazHelper::IsitInAnArray(HANDLE, masterSoundData.size()))
	{
		if (masterSoundData[HANDLE].soundData == nullptr)
		{
			FailCheck("音の再生に失敗しました\n");
			return;
		}

		//音再生
		if (masterSoundData[HANDLE].soundSorce == nullptr)
		{
			HRESULT result;
			IXAudio2SourceVoice *pSourceVoice = nullptr;
			result = xAudio2->CreateSourceVoice(&pSourceVoice, &masterSoundData[HANDLE].soundData->wfex);
			XAUDIO2_BUFFER buf{};
			buf.pAudioData = masterSoundData[HANDLE].soundData->pBuffer;
			buf.AudioBytes = masterSoundData[HANDLE].soundData->bufferSize;
			buf.Flags = XAUDIO2_END_OF_STREAM;
			if (LOOP_FLAG)
			{
				buf.LoopCount = XAUDIO2_LOOP_INFINITE;
			}
			else
			{
				buf.LoopCount = XAUDIO2_NO_LOOP_REGION;
			}

			//音量調整
			float volumeTmp = VOLUME / 100.0f;
			if (1 <= volumeTmp)
			{
				volumeTmp = 1;
			}
			pSourceVoice->SetVolume(volumeTmp);
			result = pSourceVoice->SubmitSourceBuffer(&buf);
			result = pSourceVoice->Start();
		}
		else
		{
			XAUDIO2_BUFFER buf{};
			buf.pAudioData = masterSoundData[HANDLE].soundData->pBuffer;
			buf.AudioBytes = masterSoundData[HANDLE].soundData->bufferSize;
			buf.Flags = XAUDIO2_END_OF_STREAM;
			if (LOOP_FLAG)
			{
				buf.LoopCount = XAUDIO2_LOOP_INFINITE;
			}
			else
			{
				buf.LoopCount = XAUDIO2_NO_LOOP_REGION;
			}

			//音量調整
			float volumeTmp = VOLUME / 100.0f;
			if (1 <= volumeTmp)
			{
				volumeTmp = 1;
			}
			masterSoundData[HANDLE].soundSorce->SetVolume(volumeTmp);
			masterSoundData[HANDLE].soundSorce->SubmitSourceBuffer(&buf);
			masterSoundData[HANDLE].soundSorce->Start();
		}
	}
	else
	{
		return;
	}	
}

void SoundManager::ChangeSoundMem(short HANDLE, int VOLUME)
{
	float volumeTmp = VOLUME / 100.0f;
	masterSoundData[HANDLE].soundSorce->SetVolume(volumeTmp);
}

void SoundManager::StopSoundMem(short HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, masterSoundData.size()))
	{
		if (masterSoundData[HANDLE].soundData != nullptr)
		{
			masterSoundData[HANDLE].soundSorce->Stop();
			masterSoundData[HANDLE].soundSorce->FlushSourceBuffers();
			masterSoundData[HANDLE].soundData->wfex;
		}
	}
}

void SoundManager::ReleaseSoundMem(short HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, masterSoundData.size()))
	{
		if (masterSoundData[HANDLE].soundData != nullptr)
		{	
			masterSoundData[HANDLE].soundSorce->Stop();
			masterSoundData[HANDLE].soundSorce->DestroyVoice();			
			//サウンドデータの削除
			delete masterSoundData[HANDLE].soundData->pBuffer;
			delete masterSoundData[HANDLE].soundData;			
			masterSoundData[HANDLE].soundData = nullptr;
			//ファイルパスの削除
			masterSoundData[HANDLE].filePass = "";
			//ハンドルの削除
			handle.DeleteHandle(HANDLE);
		}
	}
}
