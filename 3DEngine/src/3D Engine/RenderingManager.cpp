#include "3DEngine/EnginePCH.h"
#include "RenderingManager.h"



RenderingManager::RenderingManager()
{
	m_wnd = new Window();
	m_engine = new Engine3D();
}


RenderingManager::~RenderingManager()
{
	delete m_wnd;
	delete m_engine;
}

RenderingManager * RenderingManager::GetInstance()
{
	static RenderingManager m_instance;
	return &m_instance;
}

void RenderingManager::Init(HINSTANCE hInstance)
{
#if _DEBUG
	DEBUG = true;
#else
	DEBUG = true;
#endif
	m_hInstance = hInstance;
	WindowContext wind;
	wind.clientWidth = 1280;
	wind.clientHeight = 720;
	wind.fullscreen = false;
	wind.windowInstance = hInstance;
	wind.windowTitle = L"BitFrost 2.0";
	//Will override the Settings above
	SettingLoader::LoadWindowSettings(wind);
	m_wnd->Init(wind);

	m_engine->Init(m_wnd->GetHandler(), wind.fullscreen, wind.clientWidth, wind.clientHeight);
		
}

void RenderingManager::Update()
{
	while (m_wnd->isOpen())
	{
		InputHandler::WindowSetShowCursor();
		m_wnd->PollEvents();
	#if _DEBUG
		if (GetAsyncKeyState(int('P')))
		{
			_reloadShaders();
		}
	#endif

	}
}

void RenderingManager::UpdateSingleThread()
{
	InputHandler::WindowSetShowCursor();
	m_wnd->PollEvents();

#if _DEBUG
	if (GetAsyncKeyState(int('P')))
	{
		_reloadShaders();
	}
#endif
}

void RenderingManager::Clear()
{
	this->m_engine->Clear();
}

void RenderingManager::Flush(Camera & camera)
{
	//Draws Everything in the queue
	m_engine->Flush(camera);
	DX::g_deviceContext->ClearState();
}

void RenderingManager::Present()
{
	m_engine->Present();
}

void RenderingManager::Release()
{
	m_engine->Release();
}



Window& RenderingManager::GetWindow()
{
	return *m_wnd;
}

ProcMsg RenderingManager::GetWindowProcMsg()
{
	return m_wnd->GetWindowProcMsg();
}

void RenderingManager::_reloadShaders()
{
	DX::g_shaderManager.ReloadAllShaders();
}
