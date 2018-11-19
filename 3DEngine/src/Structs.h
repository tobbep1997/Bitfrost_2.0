#pragma once
#include <windows.h>
#include <DirectXMath.h>

struct StaticVertex
{
	DirectX::XMFLOAT4A pos;
	DirectX::XMFLOAT4A normal;
	DirectX::XMFLOAT4A tangent;
	DirectX::XMFLOAT4A uvPos;
};