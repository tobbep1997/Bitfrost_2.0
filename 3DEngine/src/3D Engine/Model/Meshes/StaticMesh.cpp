#include "3DEngine/EnginePCH.h"
#include "StaticMesh.h"


void StaticMesh::_createVertexBuffer()
{
	DX::SafeRelease(m_vertexBuffer);

	UINT32 vertexSize = sizeof(StaticVertex);
	UINT32 offSet = 0;

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(StaticVertex) * (UINT)GetVertice().size();


	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = GetRawVertice();
	HRESULT hr = DX::g_device->CreateBuffer(&bufferDesc, &vertexData, &m_vertexBuffer);
}

StaticMesh::StaticMesh()
{
}


StaticMesh::~StaticMesh()
{
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
}

const StaticVertex * StaticMesh::GetRawVertice() const
{
	return m_staticVertex.data();
}

const std::vector<StaticVertex>& StaticMesh::GetVertice() const
{
	return m_staticVertex;
}

void StaticMesh::SetVertices(std::vector<StaticVertex>& input)
{
	m_staticVertex.clear();
	m_staticVertex = input;
	_createVertexBuffer();
}

void StaticMesh::SetName(const std::string & name)
{
	this->m_meshName = name;
}

const std::string & StaticMesh::GetName() const
{
	return this->m_meshName;
}

void StaticMesh::LoadMesh(const std::string & path)
{
	using namespace DirectX;

	//TODO: Create a new mesh loader
	
	_createVertexBuffer();
}

ID3D11Buffer* StaticMesh::GetBuffer()
{
	return this->m_vertexBuffer;
}
