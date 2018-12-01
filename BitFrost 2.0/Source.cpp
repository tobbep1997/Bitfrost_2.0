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

void _alocConsole() {
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
}

void _CrtSetDbg() {
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_alocConsole();
	_CrtSetDbg();

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
	Camera cam = Camera();
	
	while (renderingManager->GetWindow().isOpen())
	{
		renderingManager->Clear();
		temp->AddRotation(0.1f * 0.1f, 0.01f * 0.1f, 0.01f * 0.1f);

		//Some Movement
		cam.CameraMovementCheat(1.f/60.0f);
		cam.GetDirection();
		//std::cout << "Dir: X" << cam.GetDirection().x << " Y: " << cam.GetDirection().y << " Z: " << cam.GetDirection().z << std::endl;
		//temp->SetPosition()
		temp->Draw();
		p_pointLight->QueueLight();
		renderingManager->Update();
		
		renderingManager->Flush(cam);

		renderingManager->Present();
	}

	delete temp;
	delete tex;
	delete p_pointLight;
	

	renderingManager->Release();
	return 0;
}
