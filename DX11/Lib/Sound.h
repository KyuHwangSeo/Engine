#pragma once
class KHSound
{
public:
	KHSound();
	~KHSound();

	static KHSound* gSound;
	static KHSound* GetInstance();
	
public:
	void Initialize();

	void LoadSoundBGM(std::string fileName, std::string key, bool loop = true);
	void LoadSoundSFX(std::string fileName, std::string key, bool loop = false);

	void PlaySoundBGM(std::string key);
	void PlaySoundSFX(std::string key);

	void VolumeChangeBGM(float volume);
	void VolumeUpBGM();
	void VolumeDownBGM();

	void VolumeChangeSFX(float volume);
	void VolumeUpSFX();
	void VolumeDownSFX();

	void VolumeChangeMaster(float volume);
	void VolumeUpMaster();
	void VolumeDownMaster();

	void StopBGM();
	void PausedBGM(bool play);

	void StopSFX();
	void PausedSFX(bool play);

	void StopAll();

	void Update();

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

