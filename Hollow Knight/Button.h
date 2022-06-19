#pragma once
#include "pch.h"
#include "Texture.h"

class Button final
{
public:
	Button(const std::string& buttonText, const Point2f& pos, bool volumeButton = false);
	~Button();

	void Draw() const;
	

	void CheckIsHoovering(const Point2f& mousePos);
	bool CheckForHit(const Point2f& mousePos) const;

private:
	Texture* m_pButtonTexture;
	Texture* m_pSelectedTexture;

	Rectf m_Hitbox;
	int m_TextSize;

	bool m_IsHoovering;
	bool m_IsVolumeButton;

	void DrawHooverTexture() const;
};

