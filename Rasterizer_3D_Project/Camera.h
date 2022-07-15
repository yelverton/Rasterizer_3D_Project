#pragma once
#include <directxmath.h>
#include <iostream>
#include <d3d11.h>
#include <wrl/client.h>
#include "Helper\BufferType.h"
#include "Helper\ErrorLog.h"

using namespace DirectX;

class Camera
{
public:
	Camera();

	void moveCamera(Camera& cam, float dt);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjection() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(XMVECTOR pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(XMFLOAT3 lookAtPos);

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetUpVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();

	bool createConstantBuffer(Camera& cam, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void adjustProjectionMatrix(float FOV, float aspectRatio, float nearZ, float farZ);
	void sendViewProjection(Camera& cam, int vertexShaderPos);
	void sendViewProjectionGS(Camera& cam, int vertexShaderPos);

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	VPmatrix VP;

	void UpdateViewMatrix();
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;

	XMMATRIX viewMatrix;
	XMMATRIX projection;
	XMMATRIX View;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_up;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
};