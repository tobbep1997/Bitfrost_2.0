#include "3DEngine/EnginePCH.h"
#include "ForwardRender.h"

ForwardRender::ForwardRender()
{

}


ForwardRender::~ForwardRender()
{

}

void ForwardRender::Init(IDXGISwapChain * swapChain, ID3D11RenderTargetView * backBufferRTV, ID3D11DepthStencilView * depthStencilView, ID3D11DepthStencilState* m_depthStencilState, ID3D11Texture2D * depthBufferTex, ID3D11SamplerState * samplerState, D3D11_VIEWPORT viewport)
{
	m_swapChain = swapChain;
	m_backBufferRTV = backBufferRTV;
	m_depthStencilView = depthStencilView;
	m_depthBufferTex = depthBufferTex;
	m_samplerState = samplerState;
	m_viewport = viewport;
	this->m_depthStencilState = m_depthStencilState;
	float c[4] = { 1.0f,0.0f,1.0f,1.0f };


	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_shaderManager.GetInputLayout(L"../Engine/EngineSource/Shader/VertexShader.hlsl"));
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
	
	_createShaders();


	_createConstantBuffer();
	_createSamplerState();

	_OutlineDepthCreate();
	m_shadowMap = new ShadowMap();
	m_shadowMap->Init(128, 128);
	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
	omDesc.RenderTarget[0].BlendEnable = true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = DX::g_device->CreateBlendState(&omDesc, &m_alphaBlend);

	DX::g_deviceContext->RSGetState(&m_standardRast);

	D3D11_RASTERIZER_DESC wfdesc{};
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	DX::g_device->CreateRasterizerState(&wfdesc, &m_wireFrame);
	DX::g_deviceContext->RSSetState(m_wireFrame);


	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	DX::g_device->CreateRasterizerState(&wfdesc, &m_disableBackFace);
	DX::g_deviceContext->RSSetState(m_disableBackFace);

	DXRHC::CreateRasterizerState(m_NUKE, FALSE, D3D11_CULL_NONE, 0, 0, FALSE);
	   
	m_2DRender = new Render2D();
	m_2DRender->Init();

	D3D11_DEPTH_STENCIL_DESC dpd{};
	dpd.DepthEnable = TRUE;
	dpd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dpd.DepthFunc = D3D11_COMPARISON_LESS;

	//Create the Depth/Stencil View
	DX::g_device->CreateDepthStencilState(&dpd, &m_particleDepthStencilState);

}

void ForwardRender::GeometryPass(Camera & camera)
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DX::g_deviceContext->OMSetBlendState(m_alphaBlend, 0, 0xffffffff);
	
	
	DX::g_deviceContext->IASetInputLayout(DX::g_shaderManager.GetInputLayout(L"../3DEngine/src/Shader/VertexShader.hlsl"));
	DX::g_deviceContext->VSSetShader(DX::g_shaderManager.GetShader<ID3D11VertexShader>(L"../3DEngine/src/Shader/VertexShader.hlsl"), nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(DX::g_shaderManager.GetShader<ID3D11PixelShader>(L"../3DEngine/src/Shader/PixelShader.hlsl"), nullptr, 0);
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
	//_SetStaticShaders();
	DrawInstanced(&camera, &DX::INSTANCING::g_instanceGroups, true);



	DX::g_deviceContext->OMSetBlendState(nullptr, 0, 0xffffffff);
}

void ForwardRender::PrePass(Camera & camera)
{
	//THIS IS VERY FUCKING IMPORTANT 
	//THIS WILL KILL EVERYTHING
	//if (m_firstRun == true)
	//{
	//	m_shaderThreads[0].join();
	//	//m_shaderThreads[1].join();
	//	m_shaderThreads[2].join();
	//	m_firstRun = false;
	//}
	//DONT FUCKING TOUCH
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(DX::g_shaderManager.GetInputLayout(L"../Engine/EngineSource/Shader/VertexShader.hlsl"));
	DX::g_deviceContext->VSSetShader(DX::g_shaderManager.GetShader<ID3D11VertexShader>(L"../Engine/EngineSource/Shader/VertexShader.hlsl"), nullptr, 0);
	DX::g_deviceContext->HSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->DSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->GSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->PSSetShader(nullptr, nullptr, 0);
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
	DX::g_deviceContext->OMSetBlendState(m_alphaBlend, 0, 0xffffffff);	
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
	
	DrawInstanced(&camera, &DX::INSTANCING::g_instanceGroups, false);



	DX::g_deviceContext->IASetInputLayout(DX::g_shaderManager.GetInputLayout(L"../Engine/EngineSource/Shader/Shaders/OutlinePrepassVertex.hlsl"));
	DX::g_deviceContext->VSSetShader(DX::g_shaderManager.GetShader<ID3D11VertexShader>(L"../Engine/EngineSource/Shader/Shaders/OutlinePrepassVertex.hlsl"), nullptr, 0);

	UINT32 vertexSize = sizeof(StaticVertex);

	UINT32 offSet = 0;
	for (int i = 0; i < DX::g_outlineQueue.size(); i++)
	{
		{
			DX::g_deviceContext->OMSetDepthStencilState(m_write0State, 0);
			ID3D11Buffer * vertexBuffer = DX::g_outlineQueue[i]->GetBuffer();

			_mapObjectOutlineBuffer(DX::g_outlineQueue[i], camera.GetPosition());
			DX::g_outlineQueue[i]->BindTextures();
			DX::g_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offSet);
			DX::g_deviceContext->Draw(DX::g_outlineQueue[i]->GetVertexSize(), 0);

		}

		{

			DX::g_deviceContext->OMSetDepthStencilState(m_write1State, 0);
			ID3D11Buffer * vertexBuffer = DX::g_outlineQueue[i]->GetBuffer();

			_mapObjectInsideOutlineBuffer(DX::g_outlineQueue[i], camera.GetPosition());
			DX::g_outlineQueue[i]->BindTextures();
			DX::g_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offSet);
			DX::g_deviceContext->Draw(DX::g_outlineQueue[i]->GetVertexSize(), 0);

		}

		DX::g_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
	}


	DX::g_deviceContext->OMSetBlendState(nullptr, 0, 0xffffffff);
}

void ForwardRender::Flush(Camera & camera)
{
	DX::g_deviceContext->OMSetDepthStencilState(m_depthStencilState, NULL);
	DX::g_deviceContext->PSSetSamplers(1, 1, &m_samplerState);
	DX::g_deviceContext->PSSetSamplers(2, 1, &m_shadowSampler);
	//this->PrePass(camera);
		
	DX::g_deviceContext->PSSetSamplers(1, 1, &m_samplerState);
	DX::g_deviceContext->PSSetSamplers(2, 1, &m_shadowSampler);
	_simpleLightCulling(camera);

	_mapLightInfoNoMatrix();
	this->m_shadowMap->MapAllLightMatrix(&DX::g_lights);
	//this->m_shadowMap->ShadowPass(this);
	this->m_shadowMap->SetSamplerAndShaderResources();
	_mapCameraBuffer(camera);
	this->GeometryPass(camera);
	//this->_OutliningPass(camera);

	//_GuardFrustumDraw();
	

	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, nullptr);
	//m_2DRender->GUIPass();
	//this->_wireFramePass(&camera);
}

void ForwardRender::Clear()
{
	float c[4] = { 0.0f,0.0f,0.0f,1.0f };

	

	DX::g_deviceContext->ClearRenderTargetView(m_backBufferRTV, c);
	DX::g_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DX::g_animatedGeometryQueue.clear();
	DX::g_lights.clear();

	DX::g_player = nullptr;

	DX::g_outlineQueue.clear();
	//this->m_shadowMap->Clear();
	DX::g_visibilityComponentQueue.clear();

	DX::g_wireFrameDrawQueue.clear();

	DX::INSTANCING::g_instanceGroups.clear();
	DX::INSTANCING::g_instanceShadowGroups.clear();
	DX::INSTANCING::g_instanceWireFrameGroups.clear();

}

void ForwardRender::Release()
{
	DX::SafeRelease(m_objectBuffer);
	DX::SafeRelease(m_cameraBuffer);
	DX::SafeRelease(m_lightBuffer);
	DX::SafeRelease(m_samplerState);
	DX::SafeRelease(m_textureBuffer);

	DX::SafeRelease(m_alphaBlend);

	DX::SafeRelease(m_standardRast);
	DX::SafeRelease(m_wireFrame);

	DX::SafeRelease(m_disableBackFace);

	DX::SafeRelease(m_shadowSampler);

	DX::SafeRelease(m_write0State);
	DX::SafeRelease(m_write1State);
	DX::SafeRelease(m_OutlineState);
	DX::SafeRelease(m_particleDepthStencilState);
	DX::SafeRelease(m_outlineBuffer);

	DX::SafeRelease(m_NUKE);
	DX::SafeRelease(m_NUKE2);

	m_shadowMap->Release();
	delete m_shadowMap;

	m_2DRender->Release();
	delete m_2DRender;
}

void ForwardRender::DrawInstanced(Camera* camera, std::vector<DX::INSTANCING::GROUP> * instanceGroup, const bool& bindTextures)
{
	using namespace DX::INSTANCING;
	if (camera)
		_mapCameraBuffer(*camera);


	size_t instanceGroupSize = instanceGroup->size();
	size_t attributeSize = 0;
	ID3D11Buffer * instanceBuffer;
	for (size_t group = 0; group < instanceGroupSize; group++)
	{
		GROUP instance = instanceGroup->at(group);

		D3D11_BUFFER_DESC instBuffDesc;
		memset(&instBuffDesc, 0, sizeof(instBuffDesc));
		instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		instBuffDesc.ByteWidth = sizeof(OBJECT) * (UINT)instance.attribs.size();
		instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		
		D3D11_SUBRESOURCE_DATA instData;
		memset(&instData, 0, sizeof(instData));
		instData.pSysMem = instance.attribs.data();
		HRESULT hr = DX::g_device->CreateBuffer(&instBuffDesc, &instData, &instanceBuffer);
		//We copy the data into the attribute part of the layout.
		// makes instancing special

		//Map Texture
		//-----------------------------------------------------------
		if (bindTextures)
		{
			//Texture * tex = new Texture();
			//std::wstring te;
			//te = L"../Namnlos.png";
			//tex->LoadSingleTexture(te.c_str());
			//tex->Bind(1);

			std::string textureName = instance.textureName;
			size_t t = textureName.find_last_of('/');
			textureName = textureName.substr(t + 1);
			Manager::g_textureManager.GetTexture(textureName)->Bind(1);
			
		}
		//-----------------------------------------------------------


		UINT offSet = 0;
		ID3D11Buffer * bufferPointers[2];
		bufferPointers[0] = instance.staticMesh->GetBuffer();
		bufferPointers[1] = instanceBuffer;

		unsigned int strides[2];
		strides[0] = sizeof(StaticVertex);
		strides[1] = sizeof(OBJECT);

		unsigned int offSets[2];
		offSets[0] = 0;
		offSets[1] = 0;

		DX::g_deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offSets);

		DX::g_deviceContext->DrawInstanced(instance.staticMesh->GetVertice().size(),
			instance.attribs.size(),
			0U,
			0U);
		DX::SafeRelease(instanceBuffer);
	}
}

void ForwardRender::_simpleLightCulling(Camera & cam)
{
	float culled = 0;
	//--------------------------------
	///Early KILL of lights
	//This is the initial culling, it will cullaway the lights that are too far away
	for (size_t i = 0; i < DX::g_lights.size(); i++)
	{
		//Loops every queued light
		if (DX::g_lights.at(i)->GetDistanceFromCamera(cam) >= m_lightCullingDistance)
		{
			DX::g_lights.erase(DX::g_lights.begin() + i);
			culled += 1;
		}
	}
	//-----------------------------------
	//Start Extream light kill
	int needToRemove = DX::g_lights.size() - m_forceCullingLimit;

	if (DX::g_lights.size() > m_forceCullingLimit)
	{
		//-----------------------------
		///Calculating the distance of the lights
		//The SortStruct can be removed but not know
		for (size_t i = 0; i < DX::g_lights.size(); i++)
		{
			DX::g_lights[i]->SetDistanceToCamera(DX::g_lights.at(i)->GetDistanceFromCamera(cam));
		}
		//
		//------------------------------
		///BubbleSorting the values. 1->...->100000
		//
		bool sorted = false;
		while (false == sorted)
		{
			sorted = true;
			for (int i = 0; i < DX::g_lights.size() - 1; ++i)
			{
				if (DX::g_lights[i]->GetDistanceToCamera() > DX::g_lights[i + 1]->GetDistanceToCamera())
				{
					sorted = false;
					PointLight * swap = DX::g_lights[i];
					DX::g_lights.at(i) = DX::g_lights.at(i + 1);
					DX::g_lights.at(i + 1) = swap;

				}
			}
		}
		//Stop BubbleSort
		//-------------------------------
		///StartPoppin the lights
		for (size_t i = 0; i < needToRemove; i++)
		{
			DX::g_lights.pop_back();
			culled++;
		}
		
		//--------------------------------
	}
	
	//TODO::
	//Check distance and check if behind then FORCE CULL THAT BITCH


}

void ForwardRender::_createConstantBuffer()
{
	HRESULT hr;
	hr = DXRHC::CreateConstantBuffer(this->m_objectBuffer, sizeof(ObjectBuffer));	
	hr = DXRHC::CreateConstantBuffer(this->m_cameraBuffer, sizeof(CameraBuffer));
	hr = DXRHC::CreateConstantBuffer(this->m_lightBuffer, sizeof(LightBuffer));
	hr = DXRHC::CreateConstantBuffer(this->m_textureBuffer, sizeof(TextureBuffer));

	hr = DXRHC::CreateConstantBuffer(this->m_outlineBuffer, sizeof(OutLineBuffer));
}

void ForwardRender::_createSamplerState()
{
	HRESULT hr = DXRHC::CreateSamplerState(m_samplerState, D3D11_TEXTURE_ADDRESS_WRAP);
	hr = DXRHC::CreateSamplerState(m_shadowSampler);
	
	DX::g_deviceContext->PSSetSamplers(1, 1, &m_samplerState);
	DX::g_deviceContext->PSSetSamplers(2, 1, &m_shadowSampler);
}

void ForwardRender::_mapObjectBuffer(Drawable * drawable)
{
	m_objectValues.worldMatrix = drawable->GetWorldmatrix();	
	DXRHC::MapBuffer(m_objectBuffer, &m_objectValues, sizeof(ObjectBuffer), 3, 1, ShaderTypes::vertex);
	


	m_textureValues.textureTileMult.x = drawable->GetTextureTileMult().x;
	m_textureValues.textureTileMult.y = drawable->GetTextureTileMult().y;

	m_textureValues.usingTexture.x = drawable->isTextureAssigned();

	m_textureValues.color = drawable->GetColor();

	DXRHC::MapBuffer(m_textureBuffer, &m_textureValues, sizeof(TextureBuffer), 7, 1, ShaderTypes::pixel);
}

void ForwardRender::_mapObjectOutlineBuffer(Drawable* drawable, const DirectX::XMFLOAT4A & pos)
{
	m_objectValues.worldMatrix = drawable->GetWorldMatrixForOutline(pos);
	DXRHC::MapBuffer(m_objectBuffer, &m_objectValues, sizeof(ObjectBuffer), 3, 1, ShaderTypes::vertex);



	m_textureValues.textureTileMult.x = drawable->GetTextureTileMult().x;
	m_textureValues.textureTileMult.y = drawable->GetTextureTileMult().y;

	m_textureValues.usingTexture.x = drawable->isTextureAssigned();

	m_textureValues.color = DirectX::XMFLOAT4A(2000,0,0,1);

	DXRHC::MapBuffer(m_textureBuffer, &m_textureValues, sizeof(TextureBuffer), 7, 1, ShaderTypes::pixel);
}

void ForwardRender::_mapObjectInsideOutlineBuffer(Drawable* drawable, const DirectX::XMFLOAT4A& pos)
{
	m_objectValues.worldMatrix = drawable->GetWorldMatrixForInsideOutline(pos);
	DXRHC::MapBuffer(m_objectBuffer, &m_objectValues, sizeof(ObjectBuffer), 3, 1, ShaderTypes::vertex);



	m_textureValues.textureTileMult.x = drawable->GetTextureTileMult().x;
	m_textureValues.textureTileMult.y = drawable->GetTextureTileMult().y;

	m_textureValues.usingTexture.x = drawable->isTextureAssigned();

	m_textureValues.color = drawable->GetColor();

	DXRHC::MapBuffer(m_textureBuffer, &m_textureValues, sizeof(TextureBuffer), 7, 1, ShaderTypes::pixel);
}

void ForwardRender::_mapCameraBuffer(Camera & camera)
{
	m_cameraValues.cameraPosition = camera.GetPosition();
	m_cameraValues.viewProjection = camera.GetViewProjection();
	DXRHC::MapBuffer(m_cameraBuffer, &m_cameraValues, sizeof(CameraBuffer), 2, 1, ShaderTypes::vertex);
	DXRHC::MapBuffer(m_cameraBuffer, &m_cameraValues, sizeof(CameraBuffer), 2, 1, ShaderTypes::pixel);
}

void ForwardRender::_mapLightInfoNoMatrix()
{
	m_lightValues.info = DirectX::XMINT4((int32_t)DX::g_lights.size(), 0, 0, 0);
	for (unsigned int i = 0; i < DX::g_lights.size(); i++)
	{
		m_lightValues.position[i] = DX::g_lights[i]->GetPosition();
		m_lightValues.color[i] = DX::g_lights[i]->GetColor();
		m_lightValues.dropOff[i] = DirectX::XMFLOAT4A(DX::g_lights[i]->GetIntensity(), DX::g_lights[i]->GetDropOff(), DX::g_lights[i]->GetPow(),0);
	}
	for (unsigned int i = (unsigned int)DX::g_lights.size(); i < 8; i++)
	{
		m_lightValues.position[i] = DirectX::XMFLOAT4A(0,0,0,0);
		m_lightValues.color[i] = DirectX::XMFLOAT4A(0,0,0,0);
		m_lightValues.dropOff[i] = DirectX::XMFLOAT4A(0, 0, 0, 0);
	}

	DXRHC::MapBuffer(m_lightBuffer, &m_lightValues, sizeof(LightBuffer), 0, 1, ShaderTypes::pixel);
}

void ForwardRender::_OutliningPass(Camera & cam)
{

	DX::g_deviceContext->IASetInputLayout(DX::g_shaderManager.GetInputLayout(L"../Engine/EngineSource/Shader/Shaders/OutlinePrepassVertex.hlsl"));
	DX::g_deviceContext->VSSetShader(DX::g_shaderManager.GetShader<ID3D11VertexShader>(L"../Engine/EngineSource/Shader/Shaders/OutlineVertexShader.hlsl"), nullptr, 0);
	DX::g_deviceContext->PSSetShader(DX::g_shaderManager.GetShader<ID3D11PixelShader>(L"../Engine/EngineSource/Shader/Shaders/OutlinePixelShader.hlsl"), nullptr, 0);
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);
	//DX::g_deviceContext->OMSetDepthStencilState(depthoutState, 0);
	UINT32 vertexSize = sizeof(StaticVertex);
	UINT32 offSet = 0;
	//_SetStaticShaders();
	//TODO
	for (unsigned int i = 0; i < DX::g_outlineQueue.size(); i++)
	{			
		m_outLineValues.outLineColor = DX::g_outlineQueue[i]->GetOutlineColor();
		//DX::g_deviceContext->PSSetShaderResources(10, 1, &m_outlineShaderRes);

		DXRHC::MapBuffer(m_outlineBuffer, &m_outLineValues, sizeof(OutLineBuffer), 8, 1, ShaderTypes::pixel);

		DX::g_deviceContext->OMSetDepthStencilState(m_OutlineState, 0);

		ID3D11Buffer * vertexBuffer = DX::g_outlineQueue[i]->GetBuffer();

		_mapObjectOutlineBuffer(DX::g_outlineQueue[i], cam.GetPosition());
		DX::g_outlineQueue[i]->BindTextures();
		DX::g_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offSet);
		DX::g_deviceContext->Draw(DX::g_outlineQueue[i]->GetVertexSize(), 0);
			

	}	   
}

void ForwardRender::_OutlineDepthCreate()
{
	D3D11_DEPTH_STENCIL_DESC depth{};
	depth.DepthEnable = FALSE;
	depth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth.StencilEnable = TRUE;

	depth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
	depth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;

	depth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	depth.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	
	depth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
	depth.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;

	DX::g_device->CreateDepthStencilState(&depth, &m_write1State);

	D3D11_DEPTH_STENCIL_DESC depth2{};
	depth2.DepthEnable = FALSE;
	depth2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth2.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth2.StencilEnable = TRUE;
		 
	depth2.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth2.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth2.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth2.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		 
	depth2.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
	depth2.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
		 
	depth2.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
	depth2.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		 
	depth2.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
	depth2.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;

	DX::g_device->CreateDepthStencilState(&depth2, &m_write0State);

	D3D11_DEPTH_STENCIL_DESC depth3{};
	depth2.DepthEnable = TRUE;
	depth2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth2.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth2.StencilEnable = TRUE;

	depth2.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth2.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth2.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	depth2.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;

	depth2.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depth2.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depth2.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth2.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	depth2.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth2.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	DX::g_device->CreateDepthStencilState(&depth2, &m_OutlineState);

	D3D11_DEPTH_STENCIL_DESC depth4{};
	depth4.DepthEnable = FALSE;
	//depth2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depth2.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth4.StencilEnable = FALSE;

	DX::g_device->CreateDepthStencilState(&depth4, &m_NUKE2);

}

void ForwardRender::_createShaders()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//World matrix
		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//Attributes
		{ "COLOR" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "UVMULT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INFO" , 0, DXGI_FORMAT_R32G32B32A32_SINT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	DX::g_shaderManager.VertexInputLayout(L"../3DEngine/src/Shader/VertexShader.hlsl", "main", inputDesc, 11);

	DX::g_shaderManager.LoadShader<ID3D11PixelShader>(L"../3DEngine/src/Shader/PixelShader.hlsl");
}

void ForwardRender::_createShadersInput()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//World matrix
		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//Attributes
		{ "COLOR" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "UVMULT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INFO" , 0, DXGI_FORMAT_R32G32B32A32_SINT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	D3D11_INPUT_ELEMENT_DESC animatedInputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "JOINTINFLUENCES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "JOINTWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	D3D11_INPUT_ELEMENT_DESC postAnimatedInputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC guardFrustumInputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ALPHA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC outlineInputDesc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}

void ForwardRender::_wireFramePass(Camera * camera)
{
	DX::g_deviceContext->RSSetState(m_wireFrame);

	DX::g_deviceContext->IASetInputLayout(DX::g_shaderManager.GetInputLayout(L"../Engine/EngineSource/Shader/VertexShader.hlsl"));
	DX::g_deviceContext->VSSetShader(DX::g_shaderManager.GetShader<ID3D11VertexShader>(L"../Engine/EngineSource/Shader/VertexShader.hlsl"), nullptr, 0);
	DX::g_deviceContext->RSSetViewports(1, &m_viewport);
	DX::g_deviceContext->OMSetRenderTargets(1, &m_backBufferRTV, nullptr);


	DX::g_deviceContext->PSSetShader(DX::g_shaderManager.GetShader<ID3D11PixelShader>(L"../Engine/EngineSource/Shader/Shaders/wireFramePixel.hlsl"), nullptr, 0);
	if (DX::INSTANCING::g_instanceWireFrameGroups.size() > 0)
		DrawInstanced(camera, &DX::INSTANCING::g_instanceWireFrameGroups, false);

}