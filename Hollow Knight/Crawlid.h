#pragma once
#include "Enemy.h"
#include "GameObjectStates.h"


class Crawlid final : public Enemy
{
public:
	Crawlid(const Rectf& boundaries);
	virtual ~Crawlid() override final;

	virtual void Draw() const override final;
	virtual void Update(float elapsedSec, const Point2f& playerPos) override final;
	/*virtual bool HandleCollision(const Rectf& actorHitbox) override final;*/

	virtual void HitEnemy() override final;
private:
	CrawlidState m_ActionState;
	Spritesheet* m_pAnimations;

	const float m_deathAirTime;
	const float m_TurnTime;
	const float m_WalkSpeed;
	const float m_HearingDistance;
	float m_AccumulatedTime;

	//sounds
	float m_CurrentVolume;
	std::string m_WalkSound;
	std::string m_HitSound;
	std::string m_DeathSound;

	void CalculateVelocity();
	void FlipImage() const;
	
	void UpdatePos(float elapsedSec);
	virtual AnimationState CalculateAnimationState() const override final;
	virtual void HandleState(float elapsedSec) override final;
	void HandleHit(float elapsedSec);
	void DrawAnimations() const;
	void CalcSoundVolume(float distance);
	void PlayWalkSound() const;
	void InitializeSounds();
};

