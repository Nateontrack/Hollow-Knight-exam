#include "TextureManager.h"
TextureManager::TextureManager()
	:m_Textures{}
{}

TextureManager::~TextureManager()
{
	for (const std::pair<std::string, Texture*>& texturePair : m_Textures)
	{
		delete texturePair.second;
	}
	m_Textures.clear();
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager* instance;
	if (instance == nullptr)
	{
		instance = new TextureManager();
	}
	return instance;
}

Texture* TextureManager::GetTexture(const std::string& filePath)
{
	//count returns a boolean: true if key is present, false if not
	if (m_Textures.count(filePath))
	{
		return m_Textures.at(filePath);
	}
	else
	{
		AddTexture(filePath);
		return m_Textures.at(filePath);
	}
}

void TextureManager::AddTexture(const std::string& filePath)
{
	std::pair<std::string, Texture*> pair{ filePath, new Texture{filePath} };
	m_Textures.insert(pair);
}

void TextureManager::DeleteStaticInstance()
{
	delete GetInstance();
}