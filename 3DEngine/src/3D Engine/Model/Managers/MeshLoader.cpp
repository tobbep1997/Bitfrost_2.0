#include "MeshLoader.h"
#include <iostream>

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::LoadMesh(const std::string& path, std::vector<StaticVertex> & fullMesh)
{
	const  aiScene * scene = aiImportFile(path.c_str(), aiProcess_Triangulate);//aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		std::cerr << "Could not load file" << path << aiGetErrorString() << std::endl;
		return false;
	}

	for (std::uint16_t meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		aiMesh * mesh = scene->mMeshes[meshIdx];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			StaticVertex tep{};
			aiVector3D pos = mesh->mVertices[i];
			aiVector3D norm = mesh->mNormals[i];
			aiVector3D tangent = mesh->mTangents[i];

			tep.pos.x = pos.x;
			tep.pos.y = pos.y;
			tep.pos.z = pos.z;

			tep.normal.x = norm.x;
			tep.normal.y = norm.y;
			tep.normal.z = norm.z;

			tep.normal.x = norm.x;
			tep.normal.y = norm.y;
			tep.normal.z = norm.z;

			tep.tangent.x = tangent.x;
			tep.tangent.y = tangent.y;
			tep.tangent.z = tangent.z;

			tep.uvPos.x = mesh->mTextureCoords[0][i].x;;
			tep.uvPos.y = mesh->mTextureCoords[0][i].y;;

			fullMesh.push_back(tep);
		}


	}


	aiReleaseImport(scene);
	return true;
}
