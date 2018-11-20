#pragma once
#include <Windows.h>

struct ProcMsg;

class Window;
class Engine3D;
class Camera;
class ImGuiManager;

class RenderingManager
{
private:
	bool	DEBUG = false;
	Window*		m_wnd;
	Engine3D*	m_engine;
	HINSTANCE m_hInstance;
	//IMGUI onlt take 1 mb in debug mode
	ImGuiManager * m_ImGuiManager;
public:
	RenderingManager();
	~RenderingManager();

	static RenderingManager * GetInstance();

	void Init(HINSTANCE hInstance);
	void Update();
	void UpdateSingleThread();

	void Clear();
	void Flush(Camera & camera);
	void Present();


	void Release();
	
	//---------------------------------
	//Explicit window related function
	Window& GetWindow();
	ProcMsg GetWindowProcMsg();

private:
	void _reloadShaders();
};

