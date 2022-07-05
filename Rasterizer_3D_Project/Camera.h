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
	void moveCamera(float dt, Camera& camera);

	const XMMATRIX& GetViewMatrix() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(float x, float y, float z);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(float x, float y);
	void AdjustRotation(float x, float y);

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();

	void setLookAtPos(XMFLOAT3 lookAtPos);
	bool createConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void adjustProjectionMatrix(float FOV, float aspectRatio, float nearZ, float farZ);
	void sendProjection(int vertexShaderPos);
	void sendViewProjection(int vertexShaderPos);

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	VPmatrix VP;

	XMMATRIX worldToView;
	XMMATRIX viewMatrix;
	XMMATRIX projection;
	XMMATRIX View;

	XMVECTOR cameraPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR lookAtPos = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);

	const XMVECTOR DEFAULT_FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_BACK = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_DOWN = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward = DEFAULT_FORWARD;
	XMVECTOR vec_back = DEFAULT_BACK;
	XMVECTOR vec_right = DEFAULT_RIGHT;
	XMVECTOR vec_left = DEFAULT_LEFT;
	XMVECTOR vec_up = DEFAULT_UP;
	XMVECTOR vec_down = DEFAULT_DOWN;

	XMFLOAT3 rotation;
	XMVECTOR rotation_Vector = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotationForward;
	XMVECTOR rotation_Vector_Forward = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX rotationMatrix;
	XMMATRIX rotationForwardMatrix;
};