#pragma comment (lib,"fmodex_vc")

#include "fmod.hpp"
#include "fmod_errors.h"
using namespace FMOD;

#include <iostream>
#include <map>
#include "Sound.h"

KHSound* KHSound::gSound = nullptr;
KHSound::KHSound()
	:m_BGMVolume(1.0f), m_SFXVolume(1.0f), m_MasterVolume(1.0f)
{

}

KHSound::~KHSound()
{

}

KHSound* KHSound::GetInstance()
{
	if (gSound == nullptr)
		gSound = new KHSound();

	return gSound;
}

void KHSound::Initialize()
{
	System_Create(&m_System);

	m_System->init(100, FMOD_INIT_NORMAL, nullptr);

	m_System->createChannelGroup("EX", &m_SFXGroup);
	m_System->createChannelGroup("BGM", &m_BGMGroup);

	m_Master->addGroup(m_SFXGroup);
	m_Master->addGroup(m_BGMGroup);
	
	m_BGMChannel->setChannelGroup(m_BGMGroup);

	for (int i = 0; i < 6; i++)
	{
		m_SFXChannel[i]->setChannelGroup(m_SFXGroup);
	}

	m_BGMRoute = "../Resource/Sound/BGM/";
	m_SFXRoute = "../Resource/Sound/SFX/";
}

void KHSound::LoadSoundBGM(std::string fileName, std::string key, bool loop)
{
	Sound* newSound;
	std::string filePath = m_BGMRoute + fileName;

	if (loop == false)
		m_System->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &newSound);
	else
		m_System->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &newSound);

	// 사운드와 채널을 같은키로 넣는다
	m_SoundList.insert(std::pair<std::string, Sound*>(key, newSound));
}

void KHSound::LoadSoundSFX(std::string fileName, std::string key, bool loop)
{
	Sound* newSound;
	std::string filePath = m_SFXRoute + fileName;

	if (loop == false)
		m_System->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &newSound);
	else
		m_System->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &newSound);

	// 사운드와 채널을 같은키로 넣는다
	m_SoundList.insert(std::pair<std::string, Sound*>(key, newSound));
}

void KHSound::PlaySoundBGM(std::string key)
{
	bool play;

	// BGM 채널에 재생중인 Sound가 있을경우 재생중인 Sound 중지..
	if (m_BGMChannel->isPlaying(&play) == FMOD_OK)
		m_BGMChannel->stop();

	// 현재 실행하려는 BGM Play..
	if (m_System->playSound(FMOD_CHANNEL_REUSE, m_SoundList[key], false, &m_BGMChannel) != FMOD_OK)
		return;

	if (m_BGMChannel != nullptr)
	{
		m_BGMChannel->setVolume(m_BGMVolume);
		m_BGMChannel->setChannelGroup(m_BGMGroup);
	}
}

void KHSound::PlaySoundSFX(std::string key)
{
	// 현재 사용하지 않는 채널을 가져온다..
	Channel* emptyChannel = FindChannel();

	if (m_System->playSound(FMOD_CHANNEL_REUSE, m_SoundList[key], false, &emptyChannel) != FMOD_OK)
		return;

	if (emptyChannel != nullptr)
	{
		m_SFXGroup->setVolume(m_SFXVolume);
		emptyChannel->setChannelGroup(m_SFXGroup);
	}
}

void KHSound::VolumeChangeBGM(float volume)
{
	if (volume < 0 && volume > 1)
		return;

	m_BGMVolume = volume;
	m_BGMGroup->setVolume(m_BGMVolume);
}

void KHSound::VolumeUpBGM()
{
	m_BGMVolume += 0.1f;
	m_BGMGroup->setVolume(m_BGMVolume);

	if (m_BGMVolume > 1)
		m_BGMVolume = 1;
}

void KHSound::VolumeDownBGM()
{
	m_BGMVolume -= 0.1f;
	m_BGMGroup->setVolume(m_BGMVolume);

	if (m_BGMVolume < 0)
		m_BGMVolume = 0;
}

void KHSound::VolumeChangeSFX(float volume)
{
	if (volume < 0 && volume > 1)
		return;

	m_SFXVolume = volume;
}

void KHSound::VolumeUpSFX()
{
	m_SFXVolume += 0.1f;

	if (m_SFXVolume > 1)
		m_SFXVolume = 1;
}

void KHSound::VolumeDownSFX()
{
	m_SFXVolume -= 0.1f;

	if (m_SFXVolume < 0)
		m_SFXVolume = 0;
}

void KHSound::VolumeChangeMaster(float volume)
{
	if (volume < 0 && volume >1)
		return;

	m_MasterVolume = volume;
	m_Master->setVolume(m_MasterVolume);
}

void KHSound::VolumeUpMaster()
{
	m_MasterVolume += 0.1f;
	m_Master->setVolume(m_MasterVolume);

	if (m_MasterVolume > 1)
		m_MasterVolume = 1;
}

void KHSound::VolumeDownMaster()
{
	m_MasterVolume -= 0.1f;
	m_Master->setVolume(m_MasterVolume);

	if (m_MasterVolume < 0)
		m_MasterVolume = 0;
}

void KHSound::StopBGM()
{
	// 모든 BGM 재생멈춤
	if (m_BGMGroup != nullptr)
		m_BGMGroup->stop();

	m_BGMChannel->stop();
}

void KHSound::PausedBGM(bool play)
{
	if (m_BGMGroup != nullptr)
		m_BGMGroup->setPaused(play);
}

void KHSound::StopSFX()
{
	if (m_SFXGroup != nullptr)
		m_SFXGroup->stop();
}

void KHSound::PausedSFX(bool play)
{
	if (m_SFXGroup != nullptr)
		m_SFXGroup->setPaused(play);
}

void KHSound::StopAll()
{
	// 모든 사운드 멈춤
	if (m_Master != nullptr)
		m_Master->stop();
}

void KHSound::Update()
{
	m_System->update();
	m_SFXGroup->setVolume(m_SFXVolume);
}

FMOD::Channel* KHSound::FindChannel()
{
	for (int i = 0; i < 6; i++)
	{
		bool play;
		m_SFXChannel[i]->isPlaying(&play);

		// 재생중인 채널이 아니면 채널을 가져온다..
		if (play != true)
			return m_SFXChannel[i];
	}

	return nullptr;
}
