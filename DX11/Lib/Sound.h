#pragma once
class KHSound
{
public:
	SOUND_DLL KHSound();
	SOUND_DLL ~KHSound();

public:
	SOUND_DLL void Initialize();

	SOUND_DLL void LoadSoundBGM(std::string fileName, std::string key, bool loop = true);
	SOUND_DLL void LoadSoundSFX(std::string fileName, std::string key, bool loop = false);

	SOUND_DLL void PlaySoundBGM(std::string key);
	SOUND_DLL void PlaySoundSFX(std::string key);

	SOUND_DLL void VolumeChangeBGM(float volume);
	SOUND_DLL void VolumeUpBGM();
	SOUND_DLL void VolumeDownBGM();

	SOUND_DLL void VolumeChangeSFX(float volume);
	SOUND_DLL void VolumeUpSFX();
	SOUND_DLL void VolumeDownSFX();

	SOUND_DLL void VolumeChangeMaster(float volume);
	SOUND_DLL void VolumeUpMaster();
	SOUND_DLL void VolumeDownMaster();

	SOUND_DLL void StopBGM();
	SOUND_DLL void PausedBGM(bool play);

	SOUND_DLL void StopSFX();
	SOUND_DLL void PausedSFX(bool play);

	SOUND_DLL void StopAll();

	SOUND_DLL void Update();

	Channel* FindChannel();

private:
	System* m_System;

	//���� ����Ʈ 
	std::map<std::string, Sound*> m_SoundList;

	//ä�ε�
	Channel* m_BGMChannel;
	Channel* m_SFXChannel[7];

	// BGM ä�� �׷�
	ChannelGroup* m_BGMGroup;

	// ȿ���� ä�� �׷�
	ChannelGroup* m_SFXGroup;

	// ������ ä�� �׷�
	ChannelGroup* m_Master;

	// BGM ���� ����
	float m_BGMVolume;

	// ȿ���� ���� ����
	float m_SFXVolume;

	// ������ ���庼��
	float m_MasterVolume;

	// ���� ���
	std::string m_BGMRoute;
	std::string m_SFXRoute;
};

