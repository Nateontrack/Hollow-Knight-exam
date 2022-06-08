#include "pch.h"
#include "Game.h"
#include <string>
#include <iostream>

Game::Game( const Window& window ) 
	:m_Window{ window },
	m_Level{},
	m_Player{ new Player{Point2f{m_Window.width / 2, 200}} }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Camera = Camera{ m_Window.width, m_Window.height, m_Player->m_CollisionHitbox };
	m_Camera.SetLevelBoundaries(m_Level.GetBoundaries());
}

void Game::Cleanup( )
{
	delete m_Player;
	m_Player = nullptr;
}

void Game::Update( float elapsedSec )
{
	MovementState first{ m_Player->GetState().action };

	m_Player->SetIsOnGround(m_Level.IsOnGround(m_Player->m_CombatHitbox));
	m_Player->Update(elapsedSec);
	m_Level.HandleCollision(m_Player->m_CombatHitbox, m_Player->m_Velocity);
	m_Level.Update(elapsedSec);

	if (first != m_Player->GetState().action) m_Player->ResetAnimations();
	//resets animation if changed in state
}

void Game::Draw( )
{
	ClearBackground( );
	glPushMatrix();
	{
		m_Camera.Transform(m_Player->m_CombatHitbox);
		m_Level.DrawBackground();
		m_Player->Draw();
		m_Level.DrawForeground();
	}
	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	//m_Player->HandleKeyDown(e);
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	/*switch ( e.keysym.sym )
	{
	case SDLK_KP_0:
		break;
	case SDLK_KP_1:
		break;
	}*/
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
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
