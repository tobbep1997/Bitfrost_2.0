#include "3DEngine/EnginePCH.h"
#include "Drawable.h"

void Drawable::_SetStaticBuffer()
{
	DX::SafeRelease(p_vertexBuffer);

	UINT32 vertexSize = sizeof(StaticVertex);
	UINT32 offSet = 0;

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(StaticVertex) * (UINT)m_staticMesh->GetVertice().size();


	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = m_staticMesh->GetRawVertice();
	HRESULT hr = DX::g_device->CreateBuffer(&bufferDesc, &vertexData, &p_vertexBuffer);



}

void Drawable::SetOutline(bool outline)
{
	m_outline = outline;
}

bool Drawable::GetOutline()
{
	return m_outline;
}

void Drawable::SetOutlineColor(const DirectX::XMFLOAT4A& color)
{
	m_outLineColor = color;
}

DirectX::XMFLOAT4A Drawable::GetOutlineColor()
{
	return m_outLineColor;
}

void Drawable::SetTransparant(const bool& bo)
{
	m_transparant = bo;
}

bool Drawable::GetTransparant()
{
	return m_transparant;
}

std::string Drawable::GetTextureName() const
{
	return std::string(this->p_texture->GetName().begin(), this->p_texture->GetName().end());
}

void Drawable::p_createBuffer()
{
	_SetStaticBuffer();
}

void Drawable::p_SetMesh(StaticMesh * staticMesh)
{
	this->m_staticMesh = staticMesh;
}

void Drawable::SetTexture(Texture * texture)
{
	this->p_texture = texture;
}

void Drawable::BindTextures()
{ 
	if (p_texture)
		p_texture->Bind(1);
}

Drawable::Drawable() : Transform()
{	
	m_staticMesh = nullptr;	
	p_vertexBuffer = nullptr;
	p_color = DirectX::XMFLOAT4A(1, 1, 1, 1);
	m_hidden = false;
	m_outLineColor = DirectX::XMFLOAT4A(1, 1, 1, 1);
	m_outline = false;
	m_transparant = false;
}

Drawable::~Drawable()
{
	DX::SafeRelease(p_vertexBuffer);
}

void Drawable::Draw()
{
	DX::INSTANCING::submitToInstance(this);
}

void Drawable::DrawWireFrame()
{
	DX::INSTANCING::submitToWireframeInstance(this);	
}

UINT Drawable::GetVertexSize()
{
	return (UINT)m_staticMesh->GetVertice().size();	
}

ID3D11Buffer * Drawable::GetBuffer()
{
	return m_staticMesh->GetBuffer();
}

StaticMesh* Drawable::GetStaticMesh()
{
	return this->m_staticMesh;
}

void Drawable::SetTextureTileMult(float u, float v)
{
	this->m_textureTileMult.x = u;
	this->m_textureTileMult.y = v;
}

const DirectX::XMFLOAT2A & Drawable::GetTextureTileMult() const
{
	return this->m_textureTileMult;
}

bool Drawable::isTextureAssigned()
{
	return p_texture;
}

void Drawable::SetModel(StaticMesh * staticMesh)
{	
	Drawable::p_SetMesh(staticMesh);
}

void Drawable::SetColor(const DirectX::XMFLOAT4A& color)
{
	this->p_color = color;
}

void Drawable::SetColor(const float& x, const float& y, const float& z, const float& w)
{
	SetColor(DirectX::XMFLOAT4A(x, y, z, w));
}

const DirectX::XMFLOAT4A& Drawable::GetColor() const
{
	return this->p_color;
}

void Drawable::SetHidden(bool hidden)
{
	m_hidden = hidden;
}

bool Drawable::GetHidden()
{
	return m_hidden;
}