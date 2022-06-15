#pragma once
#include "GameObjectStates.h"
#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "ParallaxManager.h"
#include "TextureManager.h"

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( );

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	Level m_Level;
	Player* m_Player;
	Camera m_Camera;
	ParallaxManager m_ParallaxManager;

	const float m_FadeTime;
	float m_ElapsedFadeTime;
	bool m_IsFading;
	Color4f m_Fader;

	Point2f m_StartPosPlayer;
	const float m_WindowScaleFactor;
	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void ResetKnightAnimations(const PlayerStates& previousState);
	void HandlePlayerHit();
	void CheckForFading();
	void FadeScreen(float elapsedSec);
};