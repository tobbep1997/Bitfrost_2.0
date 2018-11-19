#pragma once
#pragma warning (disable : 4267)
#include <thread>
#include <d3d11_1.h>
#include <DirectXMath.h>

struct StaticVertex;

class Drawable;
class Camera;
class PointLight;
class ShadowMap;
class Render2D;

class ForwardRender
{

	struct ObjectBuffer
	{
		DirectX::XMFLOAT4X4A worldMatrix;
	};

	struct CameraBuffer
	{
		DirectX::XMFLOAT4A cameraPosition;
		DirectX::XMFLOAT4X4A viewProjection;
	};

	struct LightBuffer
	{
		DirectX::XMINT4		info;
		DirectX::XMFLOAT4A	dropOff[8];
		DirectX::XMFLOAT4A	position[8];
		DirectX::XMFLOAT4A	color[8];
	};

	struct GuardBuffer
	{
		DirectX::XMFLOAT4X4A viewProj;
		DirectX::XMFLOAT4X4A viewProjInverse;
		DirectX::XMFLOAT4X4A worldMatrix;
	};

	struct TextureBuffer
	{
		DirectX::XMINT4		usingTexture;
		DirectX::XMFLOAT4A	textureTileMult;
		DirectX::XMFLOAT4A	color;
	};

	struct OutLineBuffer
	{
		DirectX::XMFLOAT4A outLineColor;
	};
private:
	//Standard �
	IDXGISwapChain*				m_swapChain;
	ID3D11RenderTargetView*		m_backBufferRTV;
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11Texture2D*			m_depthBufferTex;
	ID3D11SamplerState*			m_samplerState;
	ID3D11SamplerState*			m_shadowSampler;
	ID3D11DepthStencilState*	m_depthStencilState;
	ID3D11DepthStencilState*	m_particleDepthStencilState;
	

	D3D11_VIEWPORT				m_viewport;

	ID3D11Buffer* m_objectBuffer = nullptr;
	ObjectBuffer m_objectValues;

	ID3D11Buffer * m_cameraBuffer = nullptr;
	CameraBuffer m_cameraValues;

	ID3D11Buffer * m_lightBuffer = nullptr;
	LightBuffer m_lightValues;

	ID3D11Buffer * m_textureBuffer = nullptr;
	TextureBuffer m_textureValues;

	ShadowMap * m_shadowMap;
	Render2D * m_2DRender;
	
	
	float m_lightCullingDistance = 100;	
	float m_forceCullingLimit = 7;		
	std::thread m_shaderThreads[3];
	bool m_firstRun = true;
	ID3D11BlendState* m_alphaBlend;

	ID3D11RasterizerState * m_standardRast;
	ID3D11RasterizerState * m_wireFrame;
	ID3D11RasterizerState * m_disableBackFace;
	ID3D11RasterizerState * m_NUKE; //TODO: RENAME

	ID3D11Buffer * m_outlineBuffer;
	OutLineBuffer m_outLineValues;

	ID3D11DepthStencilState * m_write0State;//TODO: RENAME
	ID3D11DepthStencilState * m_write1State;//TODO: RENAME
	ID3D11DepthStencilState * m_OutlineState;
	ID3D11DepthStencilState * m_NUKE2;//TODO: RENAME


	
public:
	ForwardRender();
	~ForwardRender();

	void Init(IDXGISwapChain*				swapChain,
		ID3D11RenderTargetView*		backBufferRTV,
		ID3D11DepthStencilView*		depthStencilView,
		ID3D11DepthStencilState*	m_depthStencilState,
		ID3D11Texture2D*			depthBufferTex,
		ID3D11SamplerState*			samplerState,
		D3D11_VIEWPORT				viewport);


	void GeometryPass(Camera & camera);
	void PrePass(Camera & camera);

	void Flush(Camera & camera);
	void Clear();

	void Release();
	void DrawInstanced(Camera * camera, std::vector<DX::INSTANCING::GROUP> * instanceGroup, const bool & bindTextures = true);
private:	   
	void _simpleLightCulling(Camera & cam);

	void _createConstantBuffer();
	void _createSamplerState();
	void _mapObjectBuffer(Drawable * drawable);


	void _mapObjectOutlineBuffer(Drawable * drawable, const DirectX::XMFLOAT4A & pos);
	void _mapObjectInsideOutlineBuffer(Drawable * drawable, const DirectX::XMFLOAT4A & pos);

	void _mapCameraBuffer(Camera & camera);
	void _mapLightInfoNoMatrix();

	void _OutliningPass(Camera & cam);
	void _OutlineDepthCreate();

	void _createShaders();
	void _createShadersInput();

	void _wireFramePass(Camera * camera);
	int run = 0;
};
