#include <iostream>
#include <Windows.h>
#include "3DEngine/src/3D Engine/RenderingManager.h"
#include "3DEngine/src/Window/window.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	RenderingManager * renderingManager = RenderingManager::GetInstance();
	renderingManager->Init(hInstance);

	/*
	 * BOBBY PLS READ
	 * if .dll is missing copy from assimp/lib -->> x64/
	 * och jag har bara includat detta i bitfrost2.0
	 */

	//const  aiScene * test = aiImportFile("C:/Users/Root/Desktop/Bitfrost_2.0/BitFrost 2.0",aiProcessPreset_TargetRealtime_MaxQuality);
	//imp.ReadFile("")
	
	
	while (renderingManager->GetWindow().isOpen())
	{
		renderingManager->Update();
		renderingManager->Present();
	}
	renderingManager->Release();
	return 0;
}
