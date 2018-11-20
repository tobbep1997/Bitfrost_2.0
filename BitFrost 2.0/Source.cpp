#include <iostream>
#include <Windows.h>
#include "3DEngine/src/3D Engine/RenderingManager.h"
#include "3DEngine/src/Window/window.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	RenderingManager * renderingManager = RenderingManager::GetInstance();
	renderingManager->Init(hInstance);

	while (renderingManager->GetWindow().isOpen())
	{
		renderingManager->Update();
		renderingManager->Present();
	}
	renderingManager->Release();
	return 0;
}