#pragma once
#include <vector>
#include <d3d11_1.h>
#include <DirectXMath.h>

class Camera;
class b3Body;
class RayCastListener;

class PointLight 
{
public:
	enum ShadowDir
	{
		Y_POSITIVE = 0,
		Y_NEGATIVE = 1,
		X_POSITIVE = 2,
		X_NEGATIVE = 3,
		Z_POSITIVE = 4,
		Z_NEGATIVE = 5,
		XYZ_ALL = 6
	};

private:
	const unsigned int SHADOW_SIDES = 6U;
	const float FOV = DirectX::XM_PI * 0.5f;
	struct TourchEffectVars
	{
		double timer;
		DirectX::XMFLOAT2 current, tarGet;
		float ran;
	};

	TourchEffectVars m_tev;

	std::vector<Camera *>	m_sides;
	DirectX::XMFLOAT4A		m_position;
	DirectX::XMFLOAT4A		m_color;


	float m_nearPlane;
	float m_farPlane;
	float m_dropOff, m_intensity, m_pow;

	ID3D11ShaderResourceView *	m_shadowShaderResourceView;
	ID3D11DepthStencilView*		m_shadowDepthStencilView;
	ID3D11Texture2D*			m_shadowDepthBufferTex;


	bool m_update = false;
	bool m_firstRun = true;

	BOOL m_useSides[6];

	float m_cullingDistanceToCamera = -0.0f;

	bool m_lightOn = true;

public:
	PointLight();
	PointLight(float * translation, float * color, float intensity);
	~PointLight();

	void CreateShadowDirection(ShadowDir direction);

	void Init(DirectX::XMFLOAT4A position, DirectX::XMFLOAT4A color, float intencsity = 1.0f);

	void QueueLight();

	void SetPosition(const DirectX::XMFLOAT4A & pos);
	void SetPosition(float x, float y, float z, float w = 1);

	void SetColor(const DirectX::XMFLOAT4A & color);
	void SetColor(float x, float y, float z, float w = 1);

	void SetIntensity(float intencsity);
	void SetPower(float pow);
	void SetDropOff(float dropOff);
	
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	
	float GetDistanceFromCamera(Camera & camera);
	float GetDistanceFromObject(const DirectX::XMFLOAT4A & oPos);
	const DirectX::XMFLOAT4A & GetPosition() const;
	const DirectX::XMFLOAT4A & GetColor() const;
	const std::vector<Camera *> & GetSides() const;
	const float & GetDropOff() const;
	const float & GetPow() const;
	const float & GetIntensity() const;

	const float & GetFarPlane() const;
	const float & GetFOV() const;

	void CreateShadowDirection(const std::vector<ShadowDir> & shadowDir);

	float TourchEffect(double deltaTime, float base, float amplitude);

	ID3D11ShaderResourceView * GetSRV() const;
	ID3D11DepthStencilView * GetDSV() const;
	ID3D11Texture2D * GetTEX() const;

	void EnableSides(ShadowDir dir);
	void DisableSides(ShadowDir dir);
	//std::vector<Camera *>* GetSides();

	void SetUpdate(const bool & update);
	bool GetUpdate() const;
	void FirstRun();
	
	void Clear();

	const BOOL * useSides() const;

	void RayTrace(b3Body & object, RayCastListener * rcl);
	DirectX::XMFLOAT4A GetDir(b3Body & object) const;

	void	SetDistanceToCamera(const float & distance);
	float	GetDistanceToCamera() const;

	bool GetLightOn() const;
	void SetLightOn(bool bo);
	void SwitchLightOn();
private:
	void _createSides();
	void _createSide(const DirectX::XMFLOAT4A & dir, const DirectX::XMFLOAT4A & up);
	void _updateCameras();
	void _initDirectX(UINT width = 64U, UINT hight = 64U);
};

