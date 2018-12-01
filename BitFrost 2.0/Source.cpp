#include <iostream>
#include <Windows.h>

//#include <assimp/material.h>

#include <3DEngine/src/3D Engine/Components/Camera.h>
#include <3DEngine/src/3D Engine/Model/Texture.h>
#include "3DEngine/src/3D Engine/RenderingManager.h"
#include "3DEngine/src/Window/window.h"

#include <vector>
#include "3DEngine/src/3D Engine/Components/Base/Drawable.h"
#include "Input/InputManager/InputHandler.h"
#include "3DEngine/src/Light/PointLight.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	RenderingManager * renderingManager = RenderingManager::GetInstance();
	renderingManager->Init(hInstance);

	/*
	 * BOBBY PLS READ
	 * if .dll is missing copy from assimp/lib -->> x64/
	 * och jag har bara includat detta i bitfrost2.0
	 */

	

	Drawable * temp = new Drawable();
	temp->loadmodl();
	Texture * tex = new Texture();
	std::string te;
	std::wstring tee;
	te = "NAMLOS";
	tee = L"NAMLOS";
	tex->SetName(tee);
	tex->LoadTexture(te);
	temp->SetTexture(tex);

	temp->Draw();
	//const  aiScene * test = aiImportFile("C:/Users/Root/Desktop/Bitfrost_2.0/3x3x3.fbx",aiProcessPreset_TargetRealtime_MaxQuality);
	float pos[3] = { 2,2,2 };
	float col[3] = { 255,0,0 };
	PointLight * p_pointLight = new PointLight(pos,col,10);
	//p_pointLight->setColor(90, 112.0f, 130.0f);
	//imp.ReadFile("")

	//std::vector<float> mesh;
	////test->mMeshes[]
	//mesh.reserve(test->mNumMeshes);

	//aiMesh * meshg = test->mMeshes[0];
	
	//std::vector<int> verts;
	//aiVector3D pos = meshg->mVertices[0];
	//aiVector3D poss = meshg->mVertices[2];


	//aiReleaseImport(test);
	
	
	while (renderingManager->GetWindow().isOpen())
	{
		renderingManager->Clear();
		if (InputHandler::isKeyPressed('W'))
		{
			DirectX::XMFLOAT4A e = temp->GetPosition();
			e.z = e.z + 0.01f;
			temp->SetPosition(e);
		}
		if (InputHandler::isKeyPressed('D'))
		{
			temp->AddRotation(0, 0.1f,0);
		}
		
		//temp->SetPosition()
		temp->Draw();
		p_pointLight->QueueLight();
		renderingManager->Update();
		Camera cam = Camera();
		renderingManager->Flush(cam);

		renderingManager->Present();
	}
	renderingManager->Release();
	return 0;
}
