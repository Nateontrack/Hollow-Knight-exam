#pragma once
#include "pch.h"
#include "Texture.h"

class PowerUp
{
public:
	PowerUp(const Point2f& pos);
	bool CheckForHit(const Rectf& combatHitbox);

	void Draw() const;
private:
	Rectf m_Hitbox;
	Texture* m_pTexture;
};

