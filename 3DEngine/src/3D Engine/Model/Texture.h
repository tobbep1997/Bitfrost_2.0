#pragma once
#include <d3d11.h>
#include <stdint.h>
#include <string>

class Texture
{
private:

	std::wstring m_textureName;
public:
	Texture();
	Texture(const wchar_t* file);
	~Texture();

	HRESULT Load(const wchar_t* file);
	HRESULT LoadSingleTexture(const wchar_t * absolutePath);
	HRESULT LoadTexture(const std::string & path);

	void Bind(const uint8_t slot);
	ID3D11ShaderResourceView* m_SRV[3] = { nullptr, nullptr, nullptr };

	void SetName(const std::wstring & name);
	const std::wstring & GetName() const;
private:
};

