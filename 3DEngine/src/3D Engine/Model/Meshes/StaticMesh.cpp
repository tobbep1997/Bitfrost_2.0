#include "3DEngine/EnginePCH.h"
#include "StaticMesh.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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

	const  aiScene * test = aiImportFile("C:/Users/Root/Desktop/Bitfrost_2.0/3x3x3.fbx", aiProcessPreset_TargetRealtime_MaxQuality);

	//imp.ReadFile("")

	//std::vector<float> mesh;
	////test->mMeshes[]
	//mesh.reserve(test->mNumMeshes);

	aiMesh * meshg = test->mMeshes[0];
	
	//m_staticVerte
	//m_staticVertex.reserve(meshg->mNumVertices);
	for (unsigned int i = 0; i < meshg->mNumVertices; i++)
	{
		StaticVertex tep{};
		aiVector3D pos = meshg->mVertices[i];
		aiVector3D norm = meshg->mNormals[i];
		//aiVector3D uv = meshg->m
		tep.pos.x = pos.x;
		tep.pos.y = pos.y;
		tep.pos.z = pos.z;

		tep.normal.x = norm.x;
		tep.normal.y = norm.y;
		tep.normal.z = norm.z;

		tep.normal.x = norm.x;
		tep.normal.y = norm.y;
		tep.normal.z = norm.z;

		tep.tangent.x = 0;
		tep.tangent.y = 0;
		tep.tangent.z = 0;

		tep.uvPos.x = 1;
		tep.uvPos.y = 1;

		m_staticVertex.push_back(tep);
	}

	aiReleaseImport(test);
	_createVertexBuffer();
}

ID3D11Buffer* StaticMesh::GetBuffer()
{
	return this->m_vertexBuffer;
}
