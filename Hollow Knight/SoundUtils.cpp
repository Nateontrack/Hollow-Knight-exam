#include "SoundUtils.h"

void soundUtils::PlaySoundEffect(const std::string& soundFile, float volume)
{
	SoundManager::GetInstance()->PlaySoundEffect(SoundManager::GetInstance()->GetSoundEffect(soundFile), volume);
}

void soundUtils::PlaySoundStream(const std::string& soundFile)
{
	SoundManager::GetInstance()->PlaySoundStream(SoundManager::GetInstance()->GetSoundStream(soundFile));
}

void soundUtils::PauseSoundStream(const std::string& soundFile)
{
	SoundManager::GetInstance()->PauseSoundStream(SoundManager::GetInstance()->GetSoundStream(soundFile));
}

void soundUtils::PauseSoundEffect(const std::string& soundFile)
{
	SoundManager::GetInstance()->PauseSoundEffect(SoundManager::GetInstance()->GetSoundEffect(soundFile));
}

void soundUtils::StopSoundStream(const std::string& soundFile)
{
	SoundManager::GetInstance()->StopSoundStream(SoundManager::GetInstance()->GetSoundStream(soundFile));
}

void soundUtils::StopSoundEffect(const std::string& soundFile)
{
	SoundManager::GetInstance()->StopSoundEffect(SoundManager::GetInstance()->GetSoundEffect(soundFile));
}

bool soundUtils::GetIsPlayingEffect(const std::string& soundFile)
{
	return SoundManager::GetInstance()->GetSoundEffect(soundFile)->GetIsPlaying();
}

bool soundUtils::GetIsPlayingStream(const std::string& soundFile)
{
	return SoundManager::GetInstance()->GetSoundStream(soundFile)->IsPlaying();
}