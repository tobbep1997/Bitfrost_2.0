#pragma once
#include <windows.h>
#include <DirectXMath.h>

#include "Base/Transform.h"

class Camera : public Transform
{
	//-------------------------------------------------------------------------------------------	
	/*
		View and projection matrix
		m_viewProjection = projection x view
	*/
	DirectX::XMFLOAT4X4A m_view;
	DirectX::XMFLOAT4X4A m_projection;
	DirectX::XMFLOAT4X4A m_viewProjection;
	//-------------------------------------------------------------------------------------------	
	/*
		The nessesery varibles to create the view matrix
	*/
	DirectX::XMFLOAT4A m_direction;
	DirectX::XMFLOAT4A m_UP;
	//-------------------------------------------------------------------------------------------	
	/*
		The nessesery varibles to create the projection matrix	
	*/
	float m_fov;
	float m_aspectRatio;
	float m_nearPlane, m_farPlane;
	bool m_usingDir = true;

	DirectX::XMFLOAT2 m_lockPos;
	DirectX::XMFLOAT2 m_realLock;
	bool m_firstPos = true;

public:
	Camera(float fov = DirectX::XM_PI * 0.5f, float aspectRatio = 16.0f/9.0f, float nearPlane = 0.1f, float farPlane = 10.0f);
	~Camera();
	
	//-------------------------------------------------------------------------------------------	
	/*
		Get and Sets
	*/	
	
	/*
		The Translate functions Add the input to the position instead of changing it
		This is relative to the forward vector
	*/
	void Translate(const DirectX::XMFLOAT4A & pos);
	void Translate(float x, float y, float z, float w = 1);
	/*
		The Rotate functions rotates the direction
		This is relative to the forward vector
	*/
	void Rotate(const DirectX::XMFLOAT4A & rotation);
	void Rotate(float x, float y, float z, float w = 0);

	void SetDirection(const DirectX::XMFLOAT4A & direction);
	void SetDirection(float x, float y, float z, float w = 0);

	void SetLookTo(const DirectX::XMFLOAT4A & pos);
	void SetLookTo(float x, float y, float z, float w = 0);

	void SetUP(DirectX::XMFLOAT4A up);
	void SetUP(float x, float y, float z, float w = 0);

	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	void SetFOV(float fov);

	const float & GetFarPlane() const;
	const float & GetNearPlane() const;
	const float & GetFOV() const;

	const DirectX::XMFLOAT4A & GetDirection() const;
	DirectX::XMFLOAT4A GetRight() const;

	const DirectX::XMFLOAT4A GetYRotationEuler();
	const DirectX::XMFLOAT4A GetPitch();
	const DirectX::XMFLOAT4X4A & GetView();
	const DirectX::XMFLOAT4X4A & GetProjection() const;
	const DirectX::XMFLOAT4X4A & GetViewProjection();
	//-------------------------------------------------------------------------------------------	

	void CameraMovementCheat(const double & deltaTIme);
private:
	//-------------------------------------------------------------------------------------------	
	/*
		These create all the useful combinations of the matrixes
	*/
	void _calcViewMatrix(bool dir = true);
	void _calcProjectionMatrix();
	void _calcViewProjectionMatrix();
	//-------------------------------------------------------------------------------------------	
	/*
		Help Functions, this might be Added to a static math class if more classes needs this
	*/
	DirectX::XMFLOAT4A _Add(const DirectX::XMFLOAT4A & a, const DirectX::XMFLOAT4A & b);
public:
	DirectX::XMMATRIX ForceRotation(const DirectX::XMFLOAT4X4A& rotMatrix);
};