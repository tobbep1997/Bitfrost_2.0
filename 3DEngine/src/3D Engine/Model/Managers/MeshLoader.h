#pragma once
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include "3DEngine/src/Structs.h"

class MeshLoader
{
private:
	//Private Variables

public:
	//Public Functions
	MeshLoader();
	~MeshLoader();


	static bool LoadMesh(const std::string & path, std::vector<StaticVertex> & fullMesh);

private:
	 //Private Functions


};
