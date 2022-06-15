#include "pch.h"
#include <iostream>
#include "SoundEffect.h"

SoundEffect::SoundEffect( const std::string& path )
	:m_pMixChunk{ Mix_LoadWAV( path.c_str( ) ) },
	m_ChannelID{}
{
	if ( m_pMixChunk == nullptr )
	{
		std::string errorMsg = "SoundEffect: Failed to load " + path + ",\nSDL_mixer Error: " + Mix_GetError( );
		std::cerr << errorMsg;
	}
}
SoundEffect::~SoundEffect( )
{
	Mix_FreeChunk( m_pMixChunk );
	m_pMixChunk = nullptr;
}

bool SoundEffect::IsLoaded( ) const
{
	return m_pMixChunk != nullptr;
}

bool SoundEffect::Play(int loops)
{
	// Don't save the channel as a data member, 
	// because when it stops playing the channel becomes free
	// and available for usage by other effects
	/*if ( m_pMixChunk != nullptr )
	{
		int channel{ Mix_PlayChannel( -1, m_pMixChunk, loops ) };
		return channel == -1 ? false : true;
	}
	else
	{
		return false;
	}*/

	if (!m_pMixChunk)
	{
		return false;
	}

	m_ChannelID = Mix_PlayChannel(-1, m_pMixChunk, loops);

	if (m_ChannelID != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SoundEffect::SetVolume( int value )
{
	if ( m_pMixChunk != nullptr )
	{
		Mix_VolumeChunk( m_pMixChunk, value );
	}
}

int SoundEffect::GetVolume( ) const
{
	if ( m_pMixChunk != nullptr )
	{
		return Mix_VolumeChunk( m_pMixChunk, -1 );
	}
	else
	{
		return -1;
	}
}

void SoundEffect::StopAll( )
{
	Mix_HaltChannel(-1 );
}

void SoundEffect::PauseAll( )
{
	Mix_Pause( -1 );
}
void SoundEffect::ResumeAll( )
{
	Mix_Resume( -1 );
}

void SoundEffect::Pause()
{
	if (m_ChannelID < 0)
	{
		return;
	}
	if (Mix_GetChunk(m_ChannelID) == m_pMixChunk && Mix_Playing(m_ChannelID))
	{
		Mix_Pause(m_ChannelID);
	}
}

void SoundEffect::Stop()
{
	if (m_ChannelID < 0)
	{
		return;
	}
	if (Mix_GetChunk(m_ChannelID) == m_pMixChunk && Mix_Playing(m_ChannelID))
	{
		Mix_HaltChannel(m_ChannelID);
	}
}

bool SoundEffect::GetIsPlaying() const
{
	return Mix_Playing(m_ChannelID);
}
