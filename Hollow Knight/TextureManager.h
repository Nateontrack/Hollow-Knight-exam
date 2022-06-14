#pragma once
#include <unordered_map>
#include "pch.h"
#include "Texture.h"

class TextureManager final
{
public:
	//There's only 1 instance of every texture, gets handed out when asked with the right key
	//Is responsible for the lifetime of texture pointers
	TextureManager();
	~TextureManager();

	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;

	//singleton
	static TextureManager* GetInstance();
	static void DeleteStaticInstance();

	Texture* GetTexture(const std::string& filePath);
private:
	std::unordered_map<std::string, Texture*> m_Textures;

	void AddTexture(const std::string& filePath);
};

