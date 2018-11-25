#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <string>
#include <DirectXCollision.h>
#include "Transform.h"
#include <d3d11_3.h>

class StaticMesh;
class SkinnedMesh;
class Texture;

class Drawable : public Transform
{
private:
	StaticMesh* m_staticMesh = nullptr;
	DirectX::XMFLOAT2A m_textureTileMult = DirectX::XMFLOAT2A(1.0f,1.0f);

	bool m_hidden;
	bool m_outline;
	bool m_transparant;
	DirectX::XMFLOAT4A m_outLineColor;

protected:	
	Texture * p_texture;
	
	ID3D11Buffer * p_vertexBuffer;

	DirectX::XMFLOAT4A p_color;

	virtual void p_createBuffer();

	//Setting the mesh for the object
	virtual void p_SetMesh(StaticMesh * staticMesh);

public:
	Drawable();
	virtual ~Drawable();
	virtual void SetTexture(Texture * texture);	
	virtual void BindTextures();
	
	virtual void Draw();
	void DrawWireFrame();
	
	virtual UINT GetVertexSize();
	virtual ID3D11Buffer * GetBuffer();

	StaticMesh* GetStaticMesh();

	virtual void SetTextureTileMult(float u, float v);
	const DirectX::XMFLOAT2A & GetTextureTileMult () const;

	virtual bool isTextureAssigned();
	void SetModel(StaticMesh* staticMesh);

	virtual void SetColor(const DirectX::XMFLOAT4A & color);
	virtual void SetColor(const float & x, const float & y, const float & z, const float & w);
	virtual const DirectX::XMFLOAT4A & GetColor() const;

	virtual void SetHidden(bool hidden);
	virtual bool GetHidden();

	virtual void SetOutline(bool outline);
	virtual bool GetOutline();

	virtual void SetOutlineColor(const DirectX::XMFLOAT4A & color);
	virtual DirectX::XMFLOAT4A GetOutlineColor();

	virtual void SetTransparant(const bool & bo);
	virtual bool GetTransparant();

	std::string GetTextureName() const;

	void loadmodl();

private:
	virtual void _SetStaticBuffer();
};

