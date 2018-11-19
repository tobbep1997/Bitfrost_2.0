#include "3DEngine/EnginePCH.h"
#include "Transform.h"

float constexpr GetNewValueInNewRange(float value, float rangeMin, float rangeMax, float newRangeMin, float newRangeMax)
{
	if (value > rangeMax)
	{
		return newRangeMax;
	}
	else
	{
		return ((value - rangeMin) * ((newRangeMax - newRangeMin) / (rangeMax - rangeMin))) + newRangeMin;
	}
}

float constexpr valueInRange(float value, float min, float max)
{
	return ((value - min) / (max - min));
}


void Transform::p_calcWorldMatrix()
{
	using namespace DirectX;
	XMMATRIX translation = DirectX::XMMatrixIdentity();
	XMMATRIX scaling = DirectX::XMMatrixIdentity();
	XMMATRIX rotation = DirectX::XMMatrixIdentity();
	translation = XMMatrixTranslation(this->p_position.x, this->p_position.y, this->p_position.z);
	scaling = XMMatrixScaling(this->p_scale.x, this->p_scale.y, this->p_scale.z);

	if (p_physicsRotation._11 == INT16_MIN)
	{
		rotation = XMMatrixRotationRollPitchYaw(this->p_rotation.x, this->p_rotation.y, this->p_rotation.z);
		//rotation = rotation * p_forcedRotation;
	}
	else
	{
		rotation = XMLoadFloat3x3(&p_physicsRotation);
	}
	XMMATRIX worldMatrix;
	if (DirectX::XMMatrixIsIdentity(p_forcedWorld))
		worldMatrix = XMMatrixTranspose(XMMatrixMultiply(scaling * rotation * translation, m_modelTransform));
	else
		worldMatrix = p_forcedWorld;
	//XMMATRIX worldMatrix = XMMatrixTranspose(rotation * scaling * translation);

	if (m_parent)
		worldMatrix = XMMatrixMultiply(XMLoadFloat4x4A(&m_parent->GetWorldmatrix()), worldMatrix);

	
	XMStoreFloat4x4A(&this->p_worldMatrix, worldMatrix);

}

void Transform::p_calcWorldMatrixForOutline(const float & lenght)
{
	using namespace DirectX;

	float scaleBy = GetNewValueInNewRange(lenght, 1.0f, 30.0f, 1.3f, 3.3f);
	//scaleBy = 1.3f;
	//valueInRange(lenght, 1.3f, 2.1f);
	
	XMMATRIX translation = XMMatrixTranslation(this->p_position.x, this->p_position.y, this->p_position.z);
	XMMATRIX scaling;
	
	scaling = XMMatrixScaling(this->p_scale.x * scaleBy, this->p_scale.y * scaleBy, this->p_scale.z * scaleBy);
	
	
	XMMATRIX rotation;
	rotation = DirectX::XMMatrixIdentity();
	if (p_physicsRotation._11 == INT16_MIN)
	{
		rotation = XMMatrixRotationRollPitchYaw(this->p_rotation.x, this->p_rotation.y, this->p_rotation.z);
		//rotation = rotation * p_forcedRotation;
	}
	else
	{
		rotation = XMLoadFloat3x3(&p_physicsRotation);
	}
	XMMATRIX worldMatrix;
	if (DirectX::XMMatrixIsIdentity(p_forcedWorld))
		worldMatrix = XMMatrixTranspose(XMMatrixMultiply(scaling * rotation * translation, m_modelTransform));
	else
		worldMatrix = p_forcedWorld;
	//XMMATRIX worldMatrix = XMMatrixTranspose(rotation * scaling * translation);

	if (m_parent)
		worldMatrix = XMMatrixMultiply(XMLoadFloat4x4A(&m_parent->GetWorldmatrix()), worldMatrix);


	XMStoreFloat4x4A(&this->p_worldMatrix, worldMatrix);
}

void Transform::p_calcWorldMatrixForInsideOutline(const float& lenght)
{
	using namespace DirectX;

	float scaleBy = GetNewValueInNewRange(lenght, 1.0f, 30.0f, 1.0f, 2.5f);
	//scaleBy = 1;
	XMMATRIX translation = XMMatrixTranslation(this->p_position.x, this->p_position.y, this->p_position.z);
	XMMATRIX scaling;

	scaling = XMMatrixScaling(this->p_scale.x * scaleBy, this->p_scale.y * scaleBy, this->p_scale.z * scaleBy);


	XMMATRIX rotation;
	rotation = DirectX::XMMatrixIdentity();
	if (p_physicsRotation._11 == INT16_MIN)
	{
		rotation = XMMatrixRotationRollPitchYaw(this->p_rotation.x, this->p_rotation.y, this->p_rotation.z);
		//rotation = rotation * p_forcedRotation;
	}
	else
	{
		rotation = XMLoadFloat3x3(&p_physicsRotation);
	}
	XMMATRIX worldMatrix;
	if (DirectX::XMMatrixIsIdentity(p_forcedWorld))
		worldMatrix = XMMatrixTranspose(XMMatrixMultiply(scaling * rotation * translation, m_modelTransform));
	else
		worldMatrix = p_forcedWorld;
	//XMMATRIX worldMatrix = XMMatrixTranspose(rotation * scaling * translation);

	if (m_parent)
		worldMatrix = XMMatrixMultiply(XMLoadFloat4x4A(&m_parent->GetWorldmatrix()), worldMatrix);


	XMStoreFloat4x4A(&this->p_worldMatrix, worldMatrix);
}

Transform::Transform()
{
	p_position = DirectX::XMFLOAT4A(0, 0, 0, 1);
	p_rotation = DirectX::XMFLOAT4A(0, 0, 0, 1);
	p_scale = DirectX::XMFLOAT4A(1, 1, 1, 1);
	p_forcedWorld = DirectX::XMMatrixIdentity();
	m_modelTransform = DirectX::XMMatrixIdentity();
	
	p_physicsRotation._11 = INT16_MIN;

	m_parent = nullptr;
}


Transform::~Transform()
{
}

void Transform::SetParent(Transform & parent)
{
	this->m_parent = &parent;
}

const Transform & Transform::GetParent() const
{
	return *this->m_parent;
}

void Transform::SetPosition(const DirectX::XMFLOAT4A & pos)
{
	this->p_position = pos;
}

void Transform::SetPosition(const float & x, const float & y, const float & z, const float & w)
{
	this->SetPosition(DirectX::XMFLOAT4A(x, y, z, w));
}

void Transform::Translate(const DirectX::XMFLOAT3 & translation)
{
	this->p_position.x += translation.x;
	this->p_position.y += translation.y;
	this->p_position.z += translation.z;
}

void Transform::Translate(const float & x, const float & y, const float & z)
{
	this->p_position.x += x;
	this->p_position.y += y;
	this->p_position.z += z;
}


void Transform::SetScale(const DirectX::XMFLOAT4A & scale)
{
	this->p_scale = scale;
}

void Transform::SetScale(const float & x, const float & y, const float & z, const float & w)
{
	this->SetScale(DirectX::XMFLOAT4A(x, y, z, w));
}

void Transform::AddScale(const DirectX::XMFLOAT3 & scale)
{
	this->p_scale.x += scale.x;
	this->p_scale.y += scale.y;
	this->p_scale.z += scale.z;
}

void Transform::AddScale(const float & x, const float & y, const float & z)
{
	this->p_scale.x += x;
	this->p_scale.y += y;
	this->p_scale.z += z;
}


void Transform::SetRotation(const DirectX::XMFLOAT4A & rot)
{
	this->p_rotation = rot;
}

void Transform::SetRotation(const float & x, const float & y, const float & z, const float & w)
{
	this->SetRotation(DirectX::XMFLOAT4A(x, y, z, w));
}

void Transform::AddRotation(const DirectX::XMFLOAT4A & rot)
{
	this->p_rotation.x += rot.x;
	this->p_rotation.y += rot.y;
	this->p_rotation.z += rot.z;
	this->p_rotation.w += rot.w;
}

void Transform::AddRotation(const float & x, const float & y, const float & z, const float & w)
{
	this->p_rotation.x += x;
	this->p_rotation.y += y;
	this->p_rotation.z += z;
	this->p_rotation.w += w;
}

const Transform* Transform::GetTransform()
{
	return this;
}


const DirectX::XMFLOAT4A & Transform::GetPosition() const
{
	return this->p_position;
}

const DirectX::XMFLOAT4A & Transform::GetScale() const
{
	return this->p_scale;
}

const DirectX::XMFLOAT4A & Transform::GetEulerRotation() const
{
	return this->p_rotation;
}

DirectX::XMFLOAT4X4A Transform::GetWorldmatrix()
{
	this->p_calcWorldMatrix();

	return this->p_worldMatrix;
}

DirectX::XMFLOAT4X4A Transform::GetWorldMatrixForOutline(const DirectX::XMFLOAT4A & pos)
{
	float lenght = GetLenghtToObject(pos);

	this->p_calcWorldMatrixForOutline(lenght);

	return this->p_worldMatrix;
}

DirectX::XMFLOAT4X4A Transform::GetWorldMatrixForInsideOutline(const DirectX::XMFLOAT4A& pos)
{
	float lenght = GetLenghtToObject(pos);

	this->p_calcWorldMatrixForInsideOutline(lenght);

	return this->p_worldMatrix;
}

float Transform::GetLenghtToObject(const DirectX::XMFLOAT4A& pos)
{
	DirectX::XMVECTOR vec1 = DirectX::XMLoadFloat4A(&pos);
	DirectX::XMVECTOR vec2 = DirectX::XMLoadFloat4A(&this->p_position);
	DirectX::XMVECTOR vecSubs = DirectX::XMVectorSubtract(vec1, vec2);
	DirectX::XMVECTOR lenght = DirectX::XMVector4Length(vecSubs);

	return DirectX::XMVectorGetX(lenght);
}
