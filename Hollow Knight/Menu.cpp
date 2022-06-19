#include "Menu.h"
#include "Texture.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SoundUtils.h"

using namespace soundUtils;

Menu::Menu()
	:m_pCursor{TextureManager::GetInstance()->GetTexture("Resources/Sprites/Cursor.png")},
	m_pMenuScreen{ TextureManager::GetInstance()->GetTexture("Resources/Sprites/MenuScreen.png") },
	m_FontPath{"Resources/Fonts/Trajan Pro Regular.ttf"},
	m_StartButton{ "Start", Point2f{0,550}},
	m_ResumeButton{ "Resume", Point2f{0,550} },
	m_EffectVolumeButtonPlus{ "+", Point2f{1300,450}, true },
	m_EffectVolumeButtonMinus{ "-", Point2f{1250,450}, true },
	m_StreamVolumeButtonPlus{ "+", Point2f{1300,350}, true },
	m_StreamVolumeButtonMinus{ "-", Point2f{1250,350}, true },
	m_QuitButton{ "Quit Game", Point2f{0,250} },
	m_IsActivated{true},
	m_IsStartScreen{true},
	m_EffectsVolume{5},
	m_StreamVolume{5},
	m_TextSize{50},
	m_TextColor{1,1,1,1},
	m_ConfirmSound{"Resources/Sounds/ui_button_confirm.wav"},
	m_OptionSound{"Resources/Sounds/ui_option_click.wav"},
	m_QuitGame{}
{
	m_pVolumeEffects = new Texture{ "Effects Volume: " + std::to_string(m_EffectsVolume), m_FontPath, m_TextSize, m_TextColor};
	m_pVolumeStream = new Texture{ "Streams Volume: " + std::to_string(m_StreamVolume), m_FontPath, m_TextSize, m_TextColor };
}

Menu::~Menu()
{
	delete m_pVolumeEffects;
	delete m_pVolumeStream;
	m_pVolumeEffects = nullptr;
	m_pVolumeStream = nullptr;
}

void Menu::Draw() const
{
	if (m_IsActivated)
	{
		m_pMenuScreen->Draw();
		if (m_IsStartScreen)
		{
			m_StartButton.Draw();
		}
		else
		{
			m_ResumeButton.Draw();
		}
		m_EffectVolumeButtonPlus.Draw();
		m_EffectVolumeButtonMinus.Draw();
		m_StreamVolumeButtonPlus.Draw();
		m_StreamVolumeButtonMinus.Draw();
		m_QuitButton.Draw();
		m_pVolumeEffects->Draw(Point2f{960 - m_pVolumeEffects->GetWidth() / 2, 450});
		m_pVolumeStream->Draw(Point2f{ 960 - m_pVolumeStream->GetWidth() / 2, 350 });

		m_pCursor->Draw(Point2f{ m_MousePos.x, m_MousePos.y - m_pCursor->GetHeight() });
	}
}

void Menu::OnMouseMovement(const Point2f& pos)
{
	m_MousePos = pos;
	m_StartButton.CheckIsHoovering(pos);
	m_ResumeButton.CheckIsHoovering(pos);
	m_QuitButton.CheckIsHoovering(pos);
}

void Menu::OnMouseDown(const Point2f& pos)
{
	if (m_IsActivated)
	{
		if (m_StartButton.CheckForHit(pos) || m_ResumeButton.CheckForHit(pos))
		{
			PlaySoundEffect(m_ConfirmSound, 1);
			StartGame();
		}
		if (m_QuitButton.CheckForHit(pos))
		{
			PlaySoundEffect(m_ConfirmSound, 1);
			m_QuitGame = true;
		}

		if (m_EffectVolumeButtonPlus.CheckForHit(pos))
		{
			PlaySoundEffect(m_OptionSound, 1);
			IncrementVolumeEffects();
		}
		if (m_EffectVolumeButtonMinus.CheckForHit(pos))
		{
			PlaySoundEffect(m_OptionSound, 1);
			DecrementVolumeEffects();
		}
		if (m_StreamVolumeButtonPlus.CheckForHit(pos)) 
		{
			PlaySoundEffect(m_OptionSound, 1);
			IncrementVolumeStreams();
		}
		if (m_StreamVolumeButtonMinus.CheckForHit(pos))
		{
			PlaySoundEffect(m_OptionSound, 1);
			DecrementVolumeStreams();
		}
	}
}

void Menu::StartGame()
{
	m_IsActivated = false;
	m_IsStartScreen = false;
}

bool Menu::GetQuitGame() const
{
	return m_QuitGame;
}

void Menu::IncrementVolumeEffects()
{
	if (m_EffectsVolume < 10)
	{
		m_EffectsVolume++;
	}

	delete m_pVolumeEffects;
	m_pVolumeEffects = new Texture{ "Effects Volume: " + std::to_string(m_EffectsVolume), m_FontPath, m_TextSize, m_TextColor };

	SoundManager::GetInstance()->SetEffectsVolume(m_EffectsVolume * 10);
}

void Menu::IncrementVolumeStreams()
{
	if (m_StreamVolume < 10)
	{
		m_StreamVolume++;
	}

	delete m_pVolumeStream;
	m_pVolumeStream = new Texture{ "Streams Volume: " + std::to_string(m_StreamVolume), m_FontPath, m_TextSize, m_TextColor };

	SoundManager::GetInstance()->SetStreamVolume(m_StreamVolume * 10);
}

void Menu::DecrementVolumeEffects()
{
	if (m_EffectsVolume > 0)
	{
		m_EffectsVolume--;
	}

	delete m_pVolumeEffects;
	m_pVolumeEffects = new Texture{ "Effects Volume: " + std::to_string(m_EffectsVolume), m_FontPath, m_TextSize, m_TextColor };

	SoundManager::GetInstance()->SetEffectsVolume(m_EffectsVolume * 10);
}

void Menu::DecrementVolumeStreams()
{
	if (m_StreamVolume > 0)
	{
		m_StreamVolume--;
	}

	delete m_pVolumeStream;
	m_pVolumeStream = new Texture{ "Streams Volume: " + std::to_string(m_StreamVolume), m_FontPath, m_TextSize, m_TextColor };

	SoundManager::GetInstance()->SetStreamVolume(m_StreamVolume * 10);
}

bool Menu::GetIsPaused() const
{
	return m_IsActivated;
}

void Menu::OnEscapeKeyDown()
{
	if (m_IsActivated)
	{
		if (!m_IsStartScreen)
		{
			StartGame();
		}
	}
	else
	{
		m_IsActivated = true;
	}
}