#pragma once
#include "Texture.h"
#include "structs.h"
#include "Vector2f.h"
#include <vector>


class ParallaxManager final
{
public:
	ParallaxManager(const std::string& filePath);
	~ParallaxManager();

	//main loop
	void DrawBackground() const;
	void DrawForeground() const;
	void Update(const Vector2f& cameraTransform);

private:
	struct ParallaxLayer
	{
		Texture* pTexture;
		Point2f pos;
		float parallaxFactor;
	};

	//these are the max distances (on the z-axis) in front and behind the player
	//this psuedo 3d is used to make a realistic parallax
	const float m_NearClippingPlane;
	const float m_FarClippingPlane;

	std::vector<ParallaxLayer> m_ParallaxLayersBG;
	std::vector<ParallaxLayer> m_ParallaxLayersFG;

	float CalculateParallaxFactor(float zPos) const;

	void SortLayerVectors();

	//Parsing
	void LoadLayerDataFromFile(const std::string& filePath);
	void LoadLayerFromString(const std::string& layers);
	void CreateLayerFromString(const std::string& layer);
};

