#pragma once
#include "Animation.h"
#include "GameObjectStates.h"
#include <unordered_map>
class Spritesheet final
{
public:
	Spritesheet(const std::string& XMLFilePath, const std::string& srcImagePath);
	~Spritesheet();

	void Update(AnimationState state, float elapsedSec);
	void Draw(AnimationState state, const Point2f& centerPos) const;

	void ResetAnim(bool isAttackAnim);
	

private:
	Texture* m_pSprite;
	std::unordered_map<AnimationState, Animation*> m_pAnimations;

	void LoadAnimationsFromFile(const std::string& XMLFilePath);
	void LoadAnimationsFromString(const std::string& spritesheetData);
	void CreateAnimation(const std::string& animationData);
};