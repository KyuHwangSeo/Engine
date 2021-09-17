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

	//사운드 리스트 
	std::map<std::string, Sound*> m_SoundList;

	//채널들
	Channel* m_BGMChannel;
	Channel* m_SFXChannel[7];

	// BGM 채널 그룹
	ChannelGroup* m_BGMGroup;

	// 효과음 채널 그룹
	ChannelGroup* m_SFXGroup;

	// 마스터 채널 그룹
	ChannelGroup* m_Master;

	// BGM 사운드 볼륨
	float m_BGMVolume;

	// 효과음 사운드 볼륨
	float m_SFXVolume;

	// 마스터 사운드볼륨
	float m_MasterVolume;

	// 파일 경로
	std::string m_BGMRoute;
	std::string m_SFXRoute;
};

