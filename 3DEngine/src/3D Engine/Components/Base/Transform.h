#pragma once
#include <DirectXMath.h>



class PlayState;

class Transform
{
private:
	Transform * m_parent;
protected:
	DirectX::XMFLOAT4A p_position;
	DirectX::XMFLOAT4A p_scale;
	DirectX::XMFLOAT4A p_rotation;


	DirectX::XMMATRIX p_forcedWorld;
	DirectX::XMMATRIX m_modelTransform;
	DirectX::XMFLOAT3X3 p_physicsRotation;

	DirectX::XMFLOAT4X4A p_worldMatrix;

	void p_calcWorldMatrix();
	void p_calcWorldMatrixForOutline(const float & lenght);
	void p_calcWorldMatrixForInsideOutline(const float & lenght);
public:

	Transform();
	virtual ~Transform();

	virtual void SetParent(Transform & parent);
	virtual const Transform & GetParent() const;

	virtual void SetPosition(const DirectX::XMFLOAT4A & pos);
	virtual void SetPosition(const float & x, const float & y, const float & z, const float & w = 1.0f);
	
	virtual void Translate(const DirectX::XMFLOAT3 & translation);
	virtual void Translate(const float & x, const float & y, const float & z);

	virtual void SetScale(const DirectX::XMFLOAT4A & scale);
	virtual void SetScale(const float & x = 1.0f, const float & y = 1.0f, const float & z = 1.0f, const float & w = 1.0f);
	
	virtual void AddScale(const DirectX::XMFLOAT3 & scale);
	virtual void AddScale(const float & x, const float & y, const float & z);

	virtual void SetRotation(const DirectX::XMFLOAT4A & rot);
	virtual void SetRotation(const float & x = 0.0f, const float & y = 0.0f, const float & z = 0.0f, const float & w = 1.0f);

	virtual void AddRotation(const DirectX::XMFLOAT4A & rot);
	virtual void AddRotation(const float & x, const float & y, const float & z, const float & w = 0.0f);

	virtual const Transform * GetTransform();

	virtual const DirectX::XMFLOAT4A & GetPosition() const;
	virtual const DirectX::XMFLOAT4A & GetScale() const;
	virtual const DirectX::XMFLOAT4A & GetEulerRotation() const;

	virtual DirectX::XMFLOAT4X4A GetWorldmatrix();
	virtual DirectX::XMFLOAT4X4A GetWorldMatrixForOutline(const DirectX::XMFLOAT4A & pos);
	virtual DirectX::XMFLOAT4X4A GetWorldMatrixForInsideOutline(const DirectX::XMFLOAT4A & pos);
	
	virtual float GetLenghtToObject(const DirectX::XMFLOAT4A & pos);
};

