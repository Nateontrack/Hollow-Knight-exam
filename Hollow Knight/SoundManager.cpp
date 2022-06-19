#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager()
	:m_EffectsVolume{50},
	m_StreamVolume{70},
	m_pCurrentStream{nullptr}
{}

SoundManager::~SoundManager()
{
	for (const std::pair<std::string, SoundEffect*>& sfxPair : m_SoundEffects)
	{
		delete sfxPair.second;
	}
	m_SoundEffects.clear();

	for (const std::pair<std::string, SoundStream*>& streamPair : m_SoundStreams)
	{
		delete streamPair.second;
	}
	m_SoundStreams.clear();
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager* instance;
	if (instance == nullptr)
	{
		instance = new SoundManager();
	}
	return instance;
}

void SoundManager::DeleteInstance()
{
	delete SoundManager::GetInstance();
}

SoundEffect* SoundManager::GetSoundEffect(const std::string& soundFile)
{
	//count returns a boolean: true if key is present, false if not
	if (m_SoundEffects.count(soundFile))
	{
		return m_SoundEffects.at(soundFile);
	}
	else
	{
		AddSoundEffect(soundFile);
		return m_SoundEffects.at(soundFile);
	}
}

SoundStream* SoundManager::GetSoundStream(const std::string& soundFile)
{
	//count returns a boolean: true if key is present, false if not
	if (m_SoundStreams.count(soundFile))
	{
		return m_SoundStreams.at(soundFile);
	}
	else
	{
		AddSoundStream(soundFile);
		return m_SoundStreams.at(soundFile);
	}
}

void SoundManager::AddSoundEffect(const std::string& soundFile)
{
	std::pair<std::string, SoundEffect*> pair{ soundFile, new SoundEffect{soundFile} };
	if (pair.second->IsLoaded())
	{
		m_SoundEffects.insert(pair);
	}
	else
	{
		std::cout << "audiofile " << soundFile << " could not load\n";
	}
}

void SoundManager::AddSoundStream(const std::string& soundFile)
{
	std::pair<std::string, SoundStream*> pair{ soundFile, new SoundStream{soundFile} };
	if (pair.second->IsLoaded())
	{
		m_SoundStreams.insert(pair);
	}
	else
	{
		std::cout << "audiofile " << soundFile << " could not load\n";
	}
}

void SoundManager::PlaySoundEffect(SoundEffect* effect, float volume)
{
	//volume is a multiplier in this case, it doesnt set the general effects volume
	if (effect == nullptr)
	{
		return; //sound isnt loaded/doesnt exist
	}

	effect->SetVolume(static_cast<int>(volume * m_EffectsVolume));
	effect->Play(0);
}

void SoundManager::PlaySoundStream(SoundStream* stream)
{
	if (stream == nullptr)
	{
		return; //doesnt exist
	}

	stream->SetVolume(m_StreamVolume);
	stream->Play(true);

	m_pCurrentStream = stream;
}

int SoundManager::GetEffectsVolume() const
{
	return m_EffectsVolume;
}
void SoundManager::SetEffectsVolume(int volume)
{
	if (volume >= 0 && volume <= 100)
	{
		m_EffectsVolume = volume;
	}
}

int SoundManager::GetStreamVolume() const
{
	return m_StreamVolume;
}
void SoundManager::SetStreamVolume(int volume)
{
	if (volume >= 0 && volume <= 100)
	{
		m_StreamVolume = volume;
		m_pCurrentStream->SetVolume(volume);
	}
}

void SoundManager::PauseSoundEffect(SoundEffect* effect)
{
	effect->Pause();
}

void SoundManager::PauseSoundStream(SoundStream* stream)
{
	stream->Pause();
}

void SoundManager::StopSoundEffect(SoundEffect* effect)
{
	effect->Stop();
}

void SoundManager::StopSoundStream(SoundStream* stream)
{
	stream->Stop();
}