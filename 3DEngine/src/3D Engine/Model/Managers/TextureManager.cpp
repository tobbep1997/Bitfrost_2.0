#include "3DEngine/EnginePCH.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	for (unsigned int i = 0; i < TEXTURE_HASHTABLE_SIZE; i++)
	{
		for (unsigned int j = 0; j < m_textures[i].size(); j++)
		{
			delete m_textures[i][j];
		}
	}
	this->UnloadGUITextures();
}

void TextureManager::loadTextures(const std::string & path)
{
	Texture* tempTexture = new Texture();
	std::wstring fullPath = this->_GetFullPath(path);
	unsigned int key = this->_GetKey(fullPath);

	if (m_textures[key].size() == 0)
	{
		tempTexture->SetName(fullPath);
		
		tempTexture->Load(fullPath.c_str());

		m_textureMutex.lock();
		m_textures[key].push_back(tempTexture);
		m_textureMutex.unlock();
	}
	else
	{
		bool duplicate = false;
		for (unsigned int i = 0; i < m_textures[key].size(); i++)
		{
			if (m_textures[key].at(i)->GetName() == fullPath)
			{
				duplicate = true;
			}
		}
		if (duplicate == false)
		{
			tempTexture->SetName(fullPath);
			
			tempTexture->Load(fullPath.c_str());
			
			m_textureMutex.lock();
			m_textures[key].push_back(tempTexture);
			m_textureMutex.unlock();
		}
		else
		{
			std::cout << path << " Texture Already loaded" << std::endl;
			delete tempTexture;	
		}
	}

}

Texture * TextureManager::GetTexture(const std::string & path)
{
	std::wstring fullPath = this->_GetFullPath(path);
	unsigned int key = this->_GetKey(fullPath);

	
	for (unsigned int i = 0; i < m_textures[key].size(); i++)
	{
		if (m_textures[key][i]->GetName() == fullPath)
		{
			return m_textures[key][i];
		}
	}
	
	//This will be spammed because of the Draw calls each frame
	//std::cout << red << std::string(fullPath.begin(), fullPath.end()) << " NOT FOUND" << std::endl;
	
	//throw std::exception("TEXTURE NOT FOUND");
	return nullptr;
}

Texture * TextureManager::GetGUITextureByName(const std::wstring & name)
{
	for (auto &t : m_GuiTextures)
	{
		if (t->GetName() == name)
			return t;
	}
	
	//This will be spammed because of the Draw calls each frame
	//std::cout << red << std::string(name.begin(), name.end()) << " NOT FOUND" << std::endl;

	//throw std::exception("TEXTURE NOT FOUND");
	return nullptr;
}

void TextureManager::loadGUITexture(const std::wstring name, const std::wstring & full_path)
{
	Texture * tempTexture = new Texture();

	if (m_GuiTextures.size() > 0)
	{
		for (auto &t : m_GuiTextures)
		{
			if (t->GetName() == name)
			{
				//we already have this texture loaded 
				std::cout << std::string(name.begin(), name.end()) << " Texture Already loaded" << std::endl;	
				delete tempTexture;
				return;
			}
		}
		//new entry
		tempTexture->SetName(name);

		tempTexture->LoadSingleTexture(full_path.c_str());

		m_textureMutex.lock();
		m_GuiTextures.push_back(tempTexture);
		m_textureMutex.unlock();
	}
	else
	{
		//first entry is always free
		tempTexture->SetName(name);

		tempTexture->LoadSingleTexture(full_path.c_str());

		m_textureMutex.lock();
		m_GuiTextures.push_back(tempTexture);
		m_textureMutex.unlock();
	}
}

bool TextureManager::UnloadTexture(const std::string& path)
{
	std::wstring fullPath = this->_GetFullPath(path);
	unsigned int key = this->_GetKey(fullPath);

	for (unsigned int i = 0; i < m_textures[key].size(); i++)
	{
		if (m_textures[key].at(i)->GetName() == fullPath)
		{
			delete m_textures[key].at(i);
			m_textures[key].erase(m_textures[key].begin() + i);
			return true;
		}
	}
	return false;
}

bool TextureManager::UnloadAllTexture()
{
	for (unsigned int i = 0; i < TEXTURE_HASHTABLE_SIZE; i++)
	{
		for (unsigned int j = 0; j < m_textures[i].size(); j++)
		{
			delete m_textures[i][j];
		}
		m_textures[i].clear();
	}

	return true;
}

bool TextureManager::UnloadGUITextures()
{
	for (auto & t : m_GuiTextures)
		delete t;
	m_GuiTextures.clear();
	return false;
}

const unsigned int TextureManager::GetLoadedTextures() const
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < TEXTURE_HASHTABLE_SIZE; i++)
	{
		for (unsigned int j = 0; j < m_textures[i].size(); j++)
		{
			count++;
		}		
	}
	return count;
}

unsigned int TextureManager::_GetKey(const std::wstring & path)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < path.size(); i++)
	{
		sum += path[i];
	}
	return sum % TEXTURE_HASHTABLE_SIZE;
}

std::wstring TextureManager::_GetFullPath(const std::string & name)
{
	std::wstring tempWstring = std::wstring(name.begin(), name.end());	
	std::wstring texture = L"../AsSets/";
	texture.append(tempWstring + L"FOLDER/" + tempWstring);
	return texture;
}
