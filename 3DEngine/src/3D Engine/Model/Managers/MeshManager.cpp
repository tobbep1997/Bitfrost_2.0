#include "3DEngine/EnginePCH.h"
#include "MeshManager.h"



MeshManager::MeshManager()	{}

MeshManager::~MeshManager()
{

	for (unsigned int i = 0; i < MESH_HASHTABLE_SIZE; i++)
	{
		for (unsigned int j = 0; j < m_staticMesh[i].size(); j++)
		{
			delete m_staticMesh[i][j];
		}
		m_staticMesh[i].clear();
	}
}

bool MeshManager::loadStaticMesh(const std::string & meshName)
{
	StaticMesh* tempMesh = new StaticMesh();
	std::string fullPath = this->_GetFullPath(meshName);
	unsigned int key = this->_GetKey(fullPath);

	if (m_staticMesh[key].size() == 0)
	{		
		tempMesh->SetName(fullPath);
		tempMesh->LoadMesh(fullPath);
		m_mutexStatic.lock();
		m_staticMesh[key].push_back(tempMesh);
		m_mutexStatic.unlock();
	}
	else
	{
		bool duplicate = false;
		for (unsigned int i = 0; i < m_staticMesh[key].size(); i++)
		{
			if(m_staticMesh[key].at(i)->GetName() == fullPath)
			{
				duplicate = true;
			}
		}
		if (duplicate == false)
		{
			tempMesh->SetName(fullPath);
			tempMesh->LoadMesh(fullPath);
			m_mutexStatic.lock();
			m_staticMesh[key].push_back(tempMesh);
			m_mutexStatic.unlock();
		}
		else
		{
			//Mesh already loaded
			delete tempMesh;
			return false;
		}
	}
	
	return true;
}

StaticMesh * MeshManager::GetStaticMesh(const std::string & meshName)
{
	std::string fullPath = this->_GetFullPath(meshName);
	unsigned int key = this->_GetKey(fullPath);

	for (unsigned int i = 0; i < m_staticMesh[key].size(); i++)
	{
		if (m_staticMesh[key][i]->GetName() == fullPath)
		{
			return m_staticMesh[key][i];
		}
	}
	return nullptr;
}

bool MeshManager::UnloadStaticMesh(const std::string& meshName)
{
	std::string fullPath = this->_GetFullPath(meshName);
	unsigned int key = this->_GetKey(fullPath);

	for (unsigned int i = 0; i < m_staticMesh[key].size(); i++)
	{
		if (m_staticMesh[key].at(i)->GetName() == fullPath)
		{
			delete m_staticMesh[key].at(i);
			m_staticMesh[key].erase(m_staticMesh[key].begin() + i);
			return true;
		}
	}
	return false;
}

void MeshManager::UnloadAllMeshes()
{
	for (unsigned int i = 0; i < MESH_HASHTABLE_SIZE; i++)
	{
		for (unsigned int j = 0; j < m_staticMesh[i].size(); j++)
		{
			delete m_staticMesh[i][j];
		}
		m_staticMesh[i].clear();
	}

}

const unsigned int MeshManager::GetAllLoadedMeshes() const
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < MESH_HASHTABLE_SIZE; i++)	
		for (unsigned int j = 0; j < m_staticMesh[i].size(); j++)		
			count++;
	return count;
}

unsigned int MeshManager::_GetKey(const std::string & meshName)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < meshName.size(); i++)
	{
		sum += meshName[i];
	}
	return sum % MESH_HASHTABLE_SIZE;
}

std::string MeshManager::_GetFullPath(const std::string & meshName)
{
	std::string tempString = "../AsSets/";
	tempString.append(meshName + "FOLDER/" + meshName + ".bin");
	return tempString;
}

std::string MeshManager::_GetFullPathCollision(const std::string & meshName)
{
	std::string tempString = "../AsSets/";
	tempString.append(meshName + "FOLDER/" + meshName + "_BBOX.bin");
	return tempString;
}
