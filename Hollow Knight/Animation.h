#pragma once
#include "structs.h"
#include "Texture.h"
class Animation final
{
public:
	Animation(Texture* pTexture, const Point2f& firstPos, int nrFrames, float width, float height, bool isAttackAnim, bool isRepeating = false, int repeatFrame = 0);
	Animation(Texture* pTexture, const Point2f& firstPos, int nrFrames, float width, float height, bool isAttackAnim, float frameTime, bool isRepeating = false, int repeatFrame = 0);

	void Draw(const Point2f& centerPos) const;
	void Update(float elapsedSec);

	void Reset();

	bool GetIsAttackAnim();
private:
	const int m_NrFrames;
	const int m_RepeatFrame;
	const float m_FrameWidth;
	const float m_FrameHeight;
	const float m_FrameTime;
	const bool m_IsRepeating;
	const bool m_IsAttackAnim;
	const Point2f m_FirstPos;

	Rectf m_SrcRect;
	Texture* m_pTexture;

	float m_AccumulatedTime;
	int m_CurrentFrame;
};

