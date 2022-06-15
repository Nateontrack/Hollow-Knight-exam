#pragma once
#include "pch.h"
#include "SoundManager.h"

namespace soundUtils
{
	void PlaySoundEffect(const std::string& soundFile, float volume);
	void PlaySoundStream(const std::string& soundFile);
	void PauseSoundStream(const std::string& soundFile);
	void PauseSoundEffect(const std::string& soundFile);
	void StopSoundStream(const std::string& soundFile);
	void StopSoundEffect(const std::string& soundFile);

	bool GetIsPlayingEffect(const std::string& soundFile);
	bool GetIsPlayingStream(const std::string& soundFile);
}
