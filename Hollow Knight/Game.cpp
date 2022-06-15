#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include <string>
#include <iostream>

Game::Game( const Window& window ) 
	:m_Window{ window },
	m_Level{ "Resources/XML/GameObjects.xml", Point2f{window.width / 2, 200} },
	m_StartPosPlayer{window.width / 2, 200},
	m_ParallaxManager{"Resources/XML/ParallaxData.xml"},
	m_WindowScaleFactor{2.f / 3.f},
	m_Fader{0,0,0,0},
	m_ElapsedFadeTime{2.f},
	m_IsFading{true},
	m_FadeTime{4.f}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Player = new Player{ m_StartPosPlayer };
	m_Camera = Camera{ m_Window.width, m_Window.height, m_WindowScaleFactor, m_Player->GetCollisionFunc().combatHitbox};
	m_Camera.SetLevelBoundaries(m_Level.GetBoundaries());
}

void Game::Cleanup( )
{
	delete m_Player;
	m_Player = nullptr;
	TextureManager::DeleteStaticInstance();
	SoundManager::DeleteInstance();
}

void Game::Update( float elapsedSec )
{
	PlayerStates first{ m_Player->GetState()};

	m_Player->SetIsOnGround(m_Level.IsOnGround(m_Player->GetCollisionFunc().combatHitbox));
	m_Player->Update(elapsedSec);
	m_Level.HandleCollision(m_Player->GetCollisionFunc().combatHitbox, m_Player->GetCollisionFunc().velocity);
	HandlePlayerHit();
	m_Level.Update(elapsedSec);
	m_Camera.Update(m_Player->GetCollisionFunc().combatHitbox, elapsedSec);
	m_ParallaxManager.Update(m_Camera.GetLastCameraTransform());
	CheckForFading();
	if (m_IsFading)
	{
		FadeScreen(elapsedSec);
	}
	ResetKnightAnimations(first);
	//resets animation if changed in state
}

void Game::Draw( )
{
	ClearBackground( );
	glPushMatrix();
	{
		glScalef(m_WindowScaleFactor, m_WindowScaleFactor, 1); // 2 thirds for 720p
		m_Camera.Transform();
		m_ParallaxManager.DrawBackground();
		m_Level.DrawEntities();
		m_Player->Draw();
		m_ParallaxManager.DrawForeground();
		m_Level.DrawForeground();
	}
	glPopMatrix();
	if (m_IsFading)
	{
		utils::SetColor(m_Fader);
		utils::FillRect(Rectf{ 0,0,m_Window.width, m_Window.height });
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_Player->HandleKeyDown(e);
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_Player->HandleKeyUp(e);
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (m_Player->ExecuteAttack())
		{
			m_Level.HandleAttack(m_Player->GetCollisionFunc().attackHitbox);
		}
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::ResetKnightAnimations(const PlayerStates& previousState)
{
	if (previousState.action != m_Player->GetState().action)
	{
		m_Player->ResetAnimations(false);
	}
	if (previousState.isAttacking != m_Player->GetState().isAttacking)
	{
		m_Player->ResetAnimations(true);
	}
}

void Game::HandlePlayerHit()
{
	if (m_Level.CheckForHitSpikes(m_Player->GetCollisionFunc().combatHitbox))
	{
		m_Player->HitPlayer(true);
	}
	else if (m_Level.CheckForHitEnemies(m_Player->GetCollisionFunc().combatHitbox))
	{
		m_Player->HitPlayer(false);
	}
}

void Game::CheckForFading()
{
	PlayerStates currentState{ m_Player->GetState() };
	if (currentState.action == MovementState::spikeDeath || currentState.action == MovementState::death)
	{
		m_IsFading = true;
	}
}

void Game::FadeScreen(float elapsedSec)
{
	//switches from fade out to fade out at halfwaymark
	m_ElapsedFadeTime += elapsedSec;
	if (m_ElapsedFadeTime < m_FadeTime)
	{
		m_Fader.a = sinf(m_ElapsedFadeTime / m_FadeTime * static_cast<float>(M_PI));
	}
	else
	{
		m_Fader.a = 0;
		m_ElapsedFadeTime = 0;
		m_IsFading = false;
	}
}