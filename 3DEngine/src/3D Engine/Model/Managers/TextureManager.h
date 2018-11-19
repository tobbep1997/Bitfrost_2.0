#pragma once
#include <vector>
#include <mutex>

class Texture;

const unsigned int TEXTURE_HASHTABLE_SIZE = 13;
class TextureManager
{
private:
	std::mutex m_textureMutex;

	std::vector<Texture*> m_textures[TEXTURE_HASHTABLE_SIZE];
	std::vector<Texture*> m_GuiTextures;
public:
	TextureManager();
	~TextureManager();
	void loadTextures(const std::string & path);
	Texture* GetTexture(const std::string & path);
	Texture* GetGUITextureByName(const std::wstring& name);

	void loadGUITexture(const std::wstring name, const std::wstring & full_path);

	bool UnloadTexture(const std::string & path);
	bool UnloadAllTexture();
	bool UnloadGUITextures();

	const unsigned int GetLoadedTextures() const;
private:
	unsigned int _GetKey(const std::wstring & path);
	std::wstring _GetFullPath(const std::string & name);
};

