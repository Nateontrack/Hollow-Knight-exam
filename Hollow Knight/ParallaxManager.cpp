#include "ParallaxManager.h"
#include "utils.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace utils;

ParallaxManager::ParallaxManager(const std::string& filePath)
	:m_NearClippingPlane{50},
	m_FarClippingPlane{50}
{
	LoadLayerDataFromFile(filePath);
	SortLayerVectors();
}

ParallaxManager::~ParallaxManager()
{
	for (ParallaxLayer& layer : m_ParallaxLayersBG)
	{
		delete layer.pTexture;
		layer.pTexture = nullptr;
	}
	for (ParallaxLayer& layer : m_ParallaxLayersFG)
	{
		delete layer.pTexture;
		layer.pTexture = nullptr;
	}
}

void ParallaxManager::DrawBackground() const
{
	for (const ParallaxLayer& Layer : m_ParallaxLayersBG)
	{
		Layer.pTexture->Draw(Layer.pos);
	}
}

void ParallaxManager::DrawForeground() const
{
	for (const ParallaxLayer& Layer : m_ParallaxLayersFG)
	{
		Layer.pTexture->Draw(Layer.pos);
	}
}

void ParallaxManager::Update(const Vector2f& cameraTransform)
{
	//Moves according to predetermined parallax factor.
	for (ParallaxLayer& Layer : m_ParallaxLayersBG)
	{
		Layer.pos += cameraTransform * Layer.parallaxFactor;
	}

	for (ParallaxLayer& Layer : m_ParallaxLayersFG)
	{
		Layer.pos += cameraTransform * Layer.parallaxFactor;
	}
}


float ParallaxManager::CalculateParallaxFactor(float zPos) const
{
	if (zPos < 0)
	{
		return zPos / m_NearClippingPlane;
	}
	else if (zPos > 0)
	{
		return zPos / m_FarClippingPlane;
	}
	else
	{
		return 0;
	}
}

void ParallaxManager::SortLayerVectors()
{
	//BubbleSort
	//background
	for (size_t i = 1; i <= m_ParallaxLayersBG.size(); i++)
	{
		for (size_t j = 0; j < m_ParallaxLayersBG.size() - i; j++)
		{
			if (m_ParallaxLayersBG[j].parallaxFactor < m_ParallaxLayersBG[j + 1].parallaxFactor)
			{
				std::swap(m_ParallaxLayersBG[j], m_ParallaxLayersBG[j + 1]);
			}
		}
	}
	//foreground
	for (size_t i = 1; i <= m_ParallaxLayersFG.size(); i++)
	{
		for (size_t j = 0; j < m_ParallaxLayersFG.size() - i; j++)
		{
			if (m_ParallaxLayersFG[j].parallaxFactor < m_ParallaxLayersFG[j + 1].parallaxFactor)
			{
				std::swap(m_ParallaxLayersFG[j], m_ParallaxLayersFG[j + 1]);
			}
		}
	}
}

#pragma region Parsing
void ParallaxManager::LoadLayerDataFromFile(const std::string& filePath)
{
	//reads between <Layer> & </Layer>, passes this to creation
	std::ifstream ifs{ filePath };
	if (!ifs.good())
	{
		std::cout << filePath << " failed to load\n";
		return;
	}

	std::string layerData{};
	std::string currentLine{};
	bool isCompleted{ false };

	while (std::getline(ifs, currentLine, '\n') || !isCompleted)
	{
		layerData += currentLine;

		if (currentLine == "</LayerData>")
		{
			isCompleted = true;
		}
	}

	if (isCompleted)
	{
		LoadLayerFromString(layerData);
	}
	else
	{
		std::cout << "no layerData found in " << filePath << '\n';
	}
}

void ParallaxManager::LoadLayerFromString(const std::string& layerData)
{
	std::string layers{ GetAttributeValue("LayerData", layerData) };

	bool hasCreatedAllLayers{ false };

	while (!hasCreatedAllLayers)
	{
		std::string currentLayer{ GetAttributeValue("Layer", layers) };

		if (currentLayer == "")
		{
			hasCreatedAllLayers = true;
			break;
		}

		CreateLayerFromString(currentLayer);
		DeleteAttribute("Layer", layers);
	}
}

void ParallaxManager::CreateLayerFromString(const std::string& layer)
{
	Texture* pTexture{ new Texture(GetAttributeValue("Texture", layer)) };
	Point2f startPos{};
	float zPos{ std::stof(GetAttributeValue("ZPos", layer)) };
	startPos.x = std::stof(GetAttributeValue("StartPosX", layer));
	startPos.y = std::stof(GetAttributeValue("StartPosY", layer));
	float parallaxFactor{ CalculateParallaxFactor(zPos) };
	
	if (abs(parallaxFactor) > 1) //camera clipping safety
	{
		std::cout << "zPos is not in camera-range, aborted creation\n";
		delete pTexture;
		pTexture = nullptr;
		return;
	}

	if (parallaxFactor > 0) //parallax bigger then 0 are foreground, smaller or equal are foreground
	{
		m_ParallaxLayersBG.push_back(ParallaxLayer{ pTexture, startPos, parallaxFactor });
	}
	else
	{
		m_ParallaxLayersFG.push_back(ParallaxLayer{ pTexture, startPos, parallaxFactor });
	}
}
#pragma endregion Parsing
