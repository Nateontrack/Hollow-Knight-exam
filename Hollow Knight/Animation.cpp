#include "Animation.h"
#include <iostream>

Animation::Animation(Texture* pTexture, const Point2f& firstPos, int nrFrames, float width, float height, bool isAttackAnim, bool isRepeating, int repeatFrame)
	:m_FirstPos{firstPos},
	m_NrFrames{nrFrames},
	m_FrameWidth{width},
	m_FrameHeight{height},
	m_IsRepeating{isRepeating},
	m_RepeatFrame{repeatFrame},
	m_FrameTime{0.083333f},
	m_CurrentFrame{0},
	m_AccumulatedTime{},
	m_pTexture{pTexture},
	m_IsAttackAnim{isAttackAnim}
{
	m_SrcRect.left = m_FirstPos.x;
	m_SrcRect.bottom = m_FirstPos.y;
	m_SrcRect.width = m_FrameWidth;
	m_SrcRect.height = m_FrameHeight;
}

Animation::Animation(Texture* pTexture, const Point2f& firstPos, int nrFrames, float width, float height, bool isAttackAnim, float frameTime, bool isRepeating, int repeatFrame)
	:m_FirstPos{ firstPos },
	m_NrFrames{ nrFrames },
	m_FrameWidth{ width },
	m_FrameHeight{ height },
	m_IsRepeating{ isRepeating },
	m_RepeatFrame{ repeatFrame },
	m_FrameTime{ frameTime },
	m_CurrentFrame{ 0 },
	m_AccumulatedTime{},
	m_pTexture{ pTexture },
	m_IsAttackAnim{ isAttackAnim }
{
	m_SrcRect.left = m_FirstPos.x;
	m_SrcRect.bottom = m_FirstPos.y;
	m_SrcRect.width = m_FrameWidth;
	m_SrcRect.height = m_FrameHeight;
}

void Animation::Update(float elapsedSec)
{
	m_AccumulatedTime += elapsedSec;
	if (m_AccumulatedTime > m_FrameTime)
	{
		m_CurrentFrame++;
		m_AccumulatedTime -= m_FrameTime;

		if (m_IsRepeating)
		{
			if (m_CurrentFrame >= m_RepeatFrame)
			{
				m_CurrentFrame %= m_NrFrames - m_RepeatFrame;
				m_CurrentFrame += m_RepeatFrame;
			}
			
		}
		else if (m_CurrentFrame == m_NrFrames)
		{
			m_CurrentFrame--;
		}

		m_SrcRect.left = m_FirstPos.x + m_FrameWidth * m_CurrentFrame;
	}	
}

void Animation::Draw(const Point2f& centerPos) const
{
	Point2f bottomLeft{ centerPos.x - m_FrameWidth / 2, centerPos.y - m_FrameHeight / 2 };
	m_pTexture->Draw(bottomLeft, m_SrcRect);
}

void Animation::Reset()
{
	m_CurrentFrame = 0;
	m_AccumulatedTime = 0;
}

bool Animation::GetIsAttackAnim()
{
	return m_IsAttackAnim;
}