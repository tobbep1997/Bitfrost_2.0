#include "3DEngine/EnginePCH.h"
#include "ModelManager.h"




ModelManager::ModelManager()
{
	

}


ModelManager::~ModelManager()
{
	for (int i = 0; i < m_dynamicModel.size(); i++)
	{
		delete m_dynamicModel[i];
	}
	for (int i = 0; i < m_staticModel.size(); i++)
	{
		delete m_staticModel[i];
	}
}

void ModelManager::DrawMeshes()
{
	for (int i = 0; i < m_staticModel.size(); i++)
	{
		m_staticModel[i]->Draw();
	}
	for (int i = 0; i < m_dynamicModel.size(); i++)
	{
		m_dynamicModel[i]->Draw();
	}
}

void ModelManager::AddNewModel(StaticMesh* mesh, Texture* texture)
{
	Drawable *tempModel = new Drawable();
	tempModel->SetModel(mesh);
	tempModel->SetTexture(texture);
	m_staticModel.push_back(tempModel);
}
void ModelManager::AddNewModel(SkinnedMesh* mesh, Texture* texture)
{
	Drawable *tempModel = new Drawable();

	tempModel->SetTexture(texture);
	m_dynamicModel.push_back(tempModel);
}







