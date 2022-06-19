#pragma once
#include "pch.h"
#include "Button.h"

class Menu final
{
public:
	Menu();
	~Menu();

	void Draw() const;

	void OnMouseMovement(const Point2f& pos);
	void OnMouseDown(const Point2f& pos);
	void OnEscapeKeyDown();

	bool GetQuitGame() const;
	bool GetIsPaused() const;

private:
	std::string m_FontPath;
	Texture* m_pCursor;
	Texture* m_pMenuScreen;
	Texture* m_pVolumeEffects;
	Texture* m_pVolumeStream;

	Button m_StartButton;
	Button m_ResumeButton;
	Button m_EffectVolumeButtonPlus;
	Button m_EffectVolumeButtonMinus;
	Button m_StreamVolumeButtonPlus;
	Button m_StreamVolumeButtonMinus;
	Button m_QuitButton;

	bool m_IsActivated;
	bool m_IsStartScreen;
	bool m_QuitGame;

	int m_EffectsVolume;
	int m_StreamVolume;
	int m_TextSize;
	Point2f m_MousePos;
	Color4f m_TextColor;

	std::string m_ConfirmSound;
	std::string m_OptionSound;

	//functionality
	void StartGame();
	void IncrementVolumeEffects();
	void DecrementVolumeEffects();
	void IncrementVolumeStreams();
	void DecrementVolumeStreams();
};

