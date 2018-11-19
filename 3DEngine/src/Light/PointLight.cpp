#include "3DEngine/EnginePCH.h"
#include "PointLight.h"

PointLight::PointLight()
{
	m_tev = {
	0.0,
	{0.0f, 0.0f},
	{1.0f, 1.0f},
	5.5f
	};
	m_nearPlane = 1.0f;
	m_farPlane = 50.0f;
	_initDirectX(128U, 128U);
}

PointLight::PointLight(float * translation, float * color, float intensity)
{
	m_tev = {
	0.0,
	{0.0f, 0.0f},
	{1.0f, 1.0f},
	5.5f
	};
	m_nearPlane = 1.0f;
	m_farPlane = 20.0f;
	this->m_position = DirectX::XMFLOAT4A(translation[0], translation[1], translation[2], 1);
	this->SetColor(color[0], color[1], color[2]);
	this->m_dropOff = 1.1f;
	this->m_intensity = intensity;
	this->m_pow = 2.0f;
	//CreateShadowDirection(PointLight::Y_POSITIVE);
	CreateShadowDirection(PointLight::XYZ_ALL);
	for (int i = 0; i < 6; i++)
	{
		m_useSides[i] = TRUE;
	}
	this->m_dropOff = .5f;
	_initDirectX(128U,128U);
}

PointLight::~PointLight()
{
	for (int i = 0; i < m_sides.size(); i++)
	{
		delete m_sides[i];
	}
	DX::SafeRelease(m_shadowShaderResourceView);
	DX::SafeRelease(m_shadowDepthStencilView);
	DX::SafeRelease(m_shadowDepthBufferTex);
}

void PointLight::CreateShadowDirection(ShadowDir direction)
{
	switch (direction)
	{
	case PointLight::Y_POSITIVE:
		_createSide(DirectX::XMFLOAT4A(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4A(0.0f, 0.0f, 1.0f, 0.0f)); // UP
		break;
	case PointLight::Y_NEGATIVE:
		_createSide(DirectX::XMFLOAT4A(0.0f, -1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4A(1.0f, 0.0f, 0.0f, 0.0f)); // Down
		break;
	case PointLight::X_POSITIVE:
		_createSide(DirectX::XMFLOAT4A(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4A(0.0f, 1.0f, 0.0f, 0.0f)); // Right
		break;
	case PointLight::X_NEGATIVE:
		_createSide(DirectX::XMFLOAT4A(-1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4A(0.0f, 1.0f, 0.0f, 0.0f)); // Left
		break;
	case PointLight::Z_POSITIVE:
		_createSide(DirectX::XMFLOAT4A(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMFLOAT4A(0.0f, 1.0f, 0.0f, 0.0f)); // Forward
		break;
	case PointLight::Z_NEGATIVE:
		_createSide(DirectX::XMFLOAT4A(0.0f, 0.0f, -1.0f, 0.0f), DirectX::XMFLOAT4A(0.0f, 1.0f, 0.0f, 0.0f)); // Back
		break;
	case PointLight::XYZ_ALL:
		this->_createSides();
		break;
	}


}

void PointLight::Init(DirectX::XMFLOAT4A position, DirectX::XMFLOAT4A color, float power)
{
	this->m_position = position;
	this->m_color = color;
	this->m_dropOff = 1.0f;
	this->m_intensity = 1.0f;
	this->m_pow = 2.0f;
	//_createSides();
}

const DirectX::XMFLOAT4A & PointLight::GetPosition() const
{
	return m_position;
}

const DirectX::XMFLOAT4A & PointLight::GetColor() const
{
	return m_color;
}

const std::vector<Camera*>& PointLight::GetSides() const
{
	return m_sides;
}

const float & PointLight::GetDropOff() const
{
	return m_dropOff;
}

const float & PointLight::GetPow() const
{
	return this->m_pow;
}

const float & PointLight::GetIntensity() const
{
	return this->m_intensity;
}

const float & PointLight::GetFarPlane() const
{
	return this->m_farPlane;
}

const float & PointLight::GetFOV() const
{
	return this->FOV;
}

void PointLight::CreateShadowDirection(const std::vector<ShadowDir> & shadowDir)
{
	for (unsigned int i = 0; i < shadowDir.size(); i++)
	{
		CreateShadowDirection(shadowDir[i]);
	}
}

float PointLight::TourchEffect(double deltaTime, float base, float amplitude)
{
	m_tev.timer += deltaTime;

	if (abs(m_tev.current.x - m_tev.tarGet.x) < 0.1)
	{
		m_tev.timer = 0.0;
		m_tev.ran = (float)(rand() % 100) / 100.0f;
		m_tev.tarGet.x = m_tev.ran;
	}

	auto v1 = DirectX::XMLoadFloat2(&m_tev.current);
	auto v2 = DirectX::XMLoadFloat2(&m_tev.tarGet);
	DirectX::XMVECTOR vec;

	vec = DirectX::XMVectorLerp(v1, v2, (float)deltaTime * 5.0f);


	m_tev.current.x = DirectX::XMVectorGetX(vec);

	float temp = base + sin(m_tev.current.x) * amplitude;
	return temp;
}

ID3D11ShaderResourceView * PointLight::GetSRV() const
{
	return m_shadowShaderResourceView;
}

ID3D11DepthStencilView * PointLight::GetDSV() const
{
	return m_shadowDepthStencilView;
}

ID3D11Texture2D * PointLight::GetTEX() const
{
	return m_shadowDepthBufferTex;
}

void PointLight::EnableSides(ShadowDir dir)
{
	if (dir == XYZ_ALL)
		for (int i = 0; i < 6; i++)
			m_useSides[i] = true;
	else
		m_useSides[dir] = true;
}

void PointLight::DisableSides(ShadowDir dir)
{
	if (dir == XYZ_ALL)
		for (int i = 0; i < 6; i++)
			m_useSides[i] = false;
	else
		m_useSides[dir] = false;
}

//std::vector<Camera*>* PointLight::GetSides()
//{
//	return &m_sides;
//}

void PointLight::SetUpdate(const bool & update)
{
	this->m_update = update;
}

bool PointLight::GetUpdate() const
{
	for (int i = 0; i < 6; i++)
	{
		if (m_useSides[i])
			return true;
	}
	return false;
}

void PointLight::FirstRun()
{
	m_firstRun = false;
}

void PointLight::Clear()
{
	DX::g_deviceContext->ClearDepthStencilView(m_shadowDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

const BOOL * PointLight::useSides() const
{
	return m_useSides;
}

void PointLight::SetDistanceToCamera(const float& distance)
{
	this->m_cullingDistanceToCamera = distance;
}

float PointLight::GetDistanceToCamera() const
{
	return m_cullingDistanceToCamera;
}

bool PointLight::GetLightOn() const
{
	return m_lightOn;
}

void PointLight::SetLightOn(bool bo)
{
	m_lightOn = bo;
}

void PointLight::SwitchLightOn()
{
	m_lightOn = !m_lightOn;
}

void PointLight::QueueLight()
{
	if (m_lightOn)
	{
		DX::g_lights.push_back(this);
	}
}

void PointLight::SetPosition(const DirectX::XMFLOAT4A & pos)
{
	this->m_position = pos;
	_updateCameras();
}

void PointLight::SetPosition(float x, float y, float z, float w)
{
	this->SetPosition(DirectX::XMFLOAT4A(x, y, z, w));
}

void PointLight::SetColor(const DirectX::XMFLOAT4A & color)
{
	this->m_color = color;
}

void PointLight::SetColor(float x, float y, float z, float w)
{
	this->SetColor(DirectX::XMFLOAT4A(x / 255.0f, y / 255.0f, z / 255.0f, 1.0f));
}

void PointLight::SetIntensity(float intencsity)
{
	this->m_intensity = intencsity;
}

void PointLight::SetPower(float pow)
{
	this->m_pow = pow;
}

void PointLight::SetDropOff(float dropOff)
{
	this->m_dropOff = dropOff;
}

void PointLight::SetNearPlane(float nearPlane)
{
	for (unsigned int i = 0; i < m_sides.size(); i++)
	{
		this->m_sides[i]->SetNearPlane(nearPlane);
	}
}

void PointLight::SetFarPlane(float farPlane)
{
	for (unsigned int i = 0; i < m_sides.size(); i++)
	{
		this->m_sides[i]->SetFarPlane(farPlane);
	}
}

float PointLight::GetDistanceFromCamera(Camera& camera)
{
	DirectX::XMVECTOR vec1 = DirectX::XMLoadFloat4A(&camera.GetPosition());
	DirectX::XMVECTOR vec2 = DirectX::XMLoadFloat4A(&this->m_position);
	DirectX::XMVECTOR vecSubs = DirectX::XMVectorSubtract(vec1, vec2);
	DirectX::XMVECTOR lenght = DirectX::XMVector4Length(vecSubs);

	return DirectX::XMVectorGetX(lenght);
}

float PointLight::GetDistanceFromObject(const DirectX::XMFLOAT4A& oPos)
{
	DirectX::XMVECTOR vec1 = DirectX::XMLoadFloat4A(&oPos);
	DirectX::XMVECTOR vec2 = DirectX::XMLoadFloat4A(&this->m_position);
	DirectX::XMVECTOR vecSubs = DirectX::XMVectorSubtract(vec1, vec2);
	DirectX::XMVECTOR lenght = DirectX::XMVector4Length(vecSubs);

	return DirectX::XMVectorGetX(lenght);
}

void PointLight::_createSides()
{
	using namespace DirectX;
	Camera * cam;

	float fov = 0.5f;
	

	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetUP(0, 0, 1);
	cam->SetDirection(0, 1, 0);
	m_sides.push_back(cam);

	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetUP(1, 0, 0);
	cam->SetDirection(0, -1, 0);
	m_sides.push_back(cam);

	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetDirection(1, 0, 0);
	m_sides.push_back(cam);

	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetDirection(-1, 0, 0);
	m_sides.push_back(cam);

	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetDirection(0, 0, 1);
	m_sides.push_back(cam);

	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetDirection(0, 0, -1);
	m_sides.push_back(cam);

}

void PointLight::_createSide(const DirectX::XMFLOAT4A & dir, const DirectX::XMFLOAT4A & up)
{
	using namespace DirectX;
	Camera * cam;
	cam = new Camera(FOV, 1.0f, m_nearPlane, m_farPlane);
	cam->SetPosition(this->m_position);
	cam->SetUP(up);
	cam->SetDirection(dir);
	m_sides.push_back(cam);
}

void PointLight::_updateCameras()
{
	for (unsigned int i = 0; i < m_sides.size(); i++)
	{
		m_sides[i]->SetPosition(this->m_position);
	}
}

void PointLight::_initDirectX(UINT width, UINT hight)
{
	HRESULT hr;
	hr = DXRHC::CreateTexture2D(this->m_shadowDepthBufferTex, hight, width, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 1, 1, 0, SHADOW_SIDES, 0, 0, DXGI_FORMAT_R32_TYPELESS);
	hr = DXRHC::CreateDepthStencilView(m_shadowDepthBufferTex, this->m_shadowDepthStencilView, 0, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2DARRAY, 0, SHADOW_SIDES);
	hr = DXRHC::CreateShaderResourceView(m_shadowDepthBufferTex, m_shadowShaderResourceView, 0, DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2DARRAY, SHADOW_SIDES, 0, 0, 1);
	
}
