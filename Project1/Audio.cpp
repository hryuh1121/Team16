#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

bool Audio::Initialize()
{
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}

void Audio::PlayWave(const char* filename)
{
	HRESULT result;
	// �t�@�C���X�g���[��
	//std::ifstream file;
	FILE* file = NULL;
	// Wave�t�@�C�����J��
	//file.open(filename, std::ios_base::binary);
	//// �t�@�C���I�[�v�����s���`�F�b�N
	//if (file.fail()) {
	//	assert(0);
	//}
	if (fopen_s(&file, filename, "rb") != 0) {
		return;
	}



	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	//file.read((char*)&riff, sizeof(riff));
	fread(&riff, sizeof(riff), 1, file);

	//// �t�@�C����RIFF���`�F�b�N
	//if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
	//	assert(0);
	//}

	// Format�`�����N�̓ǂݍ���
	FormatChunk format;
	//file.read((char*)&format, sizeof(format));
	fread(&format, sizeof(format), 1, file);


	// Data�`�����N�̓ǂݍ���
	Chunk data;
	//file.read((char*)&data, sizeof(data));
	fread(&data, sizeof(data), 1, file);

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	//char* pBuffer = new char[data.size];
	//file.read(pBuffer, data.size);
	char* pBuffer = (char*)malloc(data.size);
	fread(pBuffer, data.size, 1, file);


	// Wave�t�@�C�������
	//file.close();
	fclose(file);

	WAVEFORMATEX wfex{};
	// �g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���

	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	result = pSourceVoice->Start();
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
}

void Audio::Stop()
{
	HRESULT result;
	result = pSourceVoice->Stop();

}
