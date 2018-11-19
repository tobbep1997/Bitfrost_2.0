#pragma once
#pragma warning (disable : 4172)
#include <mutex>


const uint8_t MESH_HASHTABLE_SIZE = 13;
class StaticMesh;

class MeshManager
{
private:

	std::mutex m_mutexStatic;
	std::vector<StaticMesh*> m_staticMesh[MESH_HASHTABLE_SIZE];
	
public:
	MeshManager();
	~MeshManager();
	bool loadStaticMesh(const std::string & meshName);
	StaticMesh* GetStaticMesh(const std::string & meshName);

	bool UnloadStaticMesh(const std::string & meshName);
	void UnloadAllMeshes();
	const unsigned int GetAllLoadedMeshes() const;
private:

	unsigned int _GetKey(const std::string & meshName);
	std::string _GetFullPath(const std::string & meshName);
	std::string _GetFullPathCollision(const std::string & meshName);
};

