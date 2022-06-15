#pragma once
#include "SoundStream.h"
#include "SoundEffect.h"
#include <unordered_map>

class SoundManager final
{
public:
	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;
	
	static SoundManager* GetInstance();
	static void DeleteInstance();

	SoundEffect* GetSoundEffect(const std::string& soundFile);
	SoundStream* GetSoundStream(const std::string& soundFile);

	void PlaySoundEffect(SoundEffect* effect, float volume);
	void PlaySoundStream(SoundStream* stream);
	void PauseSoundEffect(SoundEffect* effect);
	void PauseSoundStream(SoundStream* stream);
	void StopSoundEffect(SoundEffect* effect);
	void StopSoundStream(SoundStream* stream);

	int GetEffectsVolume() const;
	void SetEffectsVolume(int volume);

	int GetStreamVolume() const;
	void SetStreamVolume(int volume);
private:

	std::unordered_map<std::string, SoundEffect*> m_SoundEffects;
	std::unordered_map<std::string, SoundStream*> m_SoundStreams;

	SoundStream* m_pCurrentStream;

	int m_EffectsVolume;
	int m_StreamVolume;

	void AddSoundEffect(const std::string& soundFile);
	void AddSoundStream(const std::string& soundFile);
};

