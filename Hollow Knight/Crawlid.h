#pragma once
#include "Enemy.h"
#include "GameObjectStates.h"


class Crawlid final : public Enemy
{
public:
	Crawlid(const Rectf& hitbox, const Rectf& boundaries, const std::string& XMLFilePath, const std::string& srcImagePath);
	virtual ~Crawlid() override final;

	virtual void Draw() const override final;
	virtual void Update(float elapsedSec) override final;
	virtual void HandleCollision(const Rectf& actorHitbox) override final;
private:
	CrawlidState m_ActionState;
	Spritesheet* m_pAnimations;

	const float m_deathAirTime;
	const float m_TurnTime;
	const float m_WalkSpeed;
	float m_AccumulatedTime;

	void CalculateVelocity();
	void FlipImage() const;
	
	void UpdatePos(float elapsedSec);
	virtual AnimationState CalculateAnimationState() const override final;
	virtual void HandleState(float elapsedSec) override final;
};

