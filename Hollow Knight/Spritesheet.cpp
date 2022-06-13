#include "Spritesheet.h"
#include <fstream>
#include <string>
#include <iostream>
#include "utils.h"

using namespace utils;

Spritesheet::Spritesheet(const std::string& XMLFilePath, const std::string& srcImagePath)
	:m_pAnimations{},
	m_pSprite{new Texture(srcImagePath)}
{
	LoadAnimationsFromFile(XMLFilePath);
}

Spritesheet::~Spritesheet()
{
	delete m_pSprite;
	m_pSprite = nullptr;

	for (auto& it : m_pAnimations)
	{
		delete it.second;
		it.second = nullptr;
	}

	m_pAnimations.clear();
}

void Spritesheet::Update(AnimationState state, float elapsedSec)
{
//state manipulation + animation updating
m_pAnimations.at(state)->Update(elapsedSec);
}

void Spritesheet::Draw(AnimationState state, const Point2f& centerPos) const
{
	m_pAnimations.at(state)->Draw(centerPos);
}

void Spritesheet::ResetAnim(bool isAttackAnim)
{
	if (isAttackAnim)
	{
		for (auto& it : m_pAnimations)
		{
			it.second->Reset();
		}
	}
	else
	{
		for (auto& it : m_pAnimations)
		{
			if (!it.second->GetIsAttackAnim())
			{
				it.second->Reset();
			}
		}
	}
}

#pragma region AnimationLoading

void Spritesheet::LoadAnimationsFromFile(const std::string& XMLFilePath)
{
	//reads between <Spritesheet> & </Spritesheet>, passes this to creation
	std::ifstream ifs{ XMLFilePath };
	if (!ifs.good())
	{
		std::cout << XMLFilePath << " failed to load\n";
		return;
	}

	std::string spritesheetData{};
	std::string currentLine{};
	bool isCompleted{ false };

	while (std::getline(ifs, currentLine, '\n') || !isCompleted)
	{
		spritesheetData += currentLine;

		if (currentLine == "</Spritesheet>")
		{
			isCompleted = true;
		}
	}

	if (isCompleted)
	{
		LoadAnimationsFromString(spritesheetData);
	}
	else
	{
		std::cout << "no layerData found in " << XMLFilePath << '\n';
	}
}

void Spritesheet::LoadAnimationsFromString(const std::string& spritesheetData)
{
	std::string animations{ GetAttributeValue("Spritesheet", spritesheetData) };

	bool hasCreatedAllAnims{ false };

	while (!hasCreatedAllAnims)
	{
		std::string currentAnimation{ GetAttributeValue("Animation", animations) };

		if (currentAnimation == "")
		{
			hasCreatedAllAnims = true;
			break;
		}

		CreateAnimation(currentAnimation);
		DeleteAttribute("Animation", animations);
	}
}

void Spritesheet::CreateAnimation(const std::string& animationData)
{
	AnimationState id{ static_cast<AnimationState>(std::stoi(GetAttributeValue("id", animationData))) };
	Point2f firstPos{ std::stof(GetAttributeValue("firstPosX", animationData)),
	m_pSprite->GetHeight() - std::stof(GetAttributeValue("firstPosY", animationData)) };
	int nrFrames{ std::stoi(GetAttributeValue("nrFrames", animationData)) };
	float width{ std::stof(GetAttributeValue("width", animationData)) };
	float height{ std::stof(GetAttributeValue("height", animationData)) };
	bool isRepeating{ static_cast<bool>(std::stoi(GetAttributeValue("isRepeating", animationData))) };
	bool isAttackAnim{ static_cast<bool>(std::stoi(GetAttributeValue("isAttackAnim", animationData))) };
	int repeatFrame{ std::stoi(GetAttributeValue("repeatFrame", animationData)) };

	if (GetAttributeValue("frameTime", animationData) == "default")
	{
		m_pAnimations.insert(std::make_pair(id, new Animation{ m_pSprite, firstPos, nrFrames, width, height, isAttackAnim, isRepeating, repeatFrame }));
	}
	else
	{
		float frameTime{ std::stof(GetAttributeValue("frameTime", animationData)) };
		m_pAnimations.insert(std::make_pair(id, new Animation{ m_pSprite, firstPos, nrFrames, width, height, isAttackAnim, frameTime, isRepeating, repeatFrame }));
	}
}

#pragma endregion AnimationLoading

