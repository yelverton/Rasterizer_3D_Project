#include "Camera.h"

Camera::Camera()
{
}

void Camera::moveCamera(float dt, Camera &camera)
{
	if (GetAsyncKeyState('W')) {
		vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD, rotationForwardMatrix);
		cameraPos += vec_forward * 20 * dt;
		lookAtPos += vec_forward * 20 * dt;
	}
	else if (GetAsyncKeyState('S')) {
		vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD, rotationForwardMatrix);
		cameraPos -= vec_forward * 20 * dt;
		lookAtPos -= vec_forward * 20 * dt;
	}

	if (GetAsyncKeyState('D')) {
		vec_right = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMatrix);
		cameraPos += vec_right * 20 * dt;
		lookAtPos += vec_right * 20 * dt;
	}
	else if (GetAsyncKeyState('A')) {	
		vec_right = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMatrix);
		cameraPos -= vec_right * 20 * dt;
		lookAtPos -= vec_right * 20 * dt;
	}

	if (GetAsyncKeyState('E')) {
		AdjustRotation(2 * dt, 0.0f);
	}
	else if (GetAsyncKeyState('Q')) {
		AdjustRotation(-(2 * dt), 0.0f);
	}

	/*if (GetAsyncKeyState('K')) {
		cam.AdjustRotation(cam.GetRightVector() * dt * 50);
	}
	else if (GetAsyncKeyState('I')) {
		cam.AdjustRotation(cam.GetRightVector() * dt * -50);
	}

	if (GetAsyncKeyState('J')) {
		cam.AdjustRotation(0, -0.005, 0);
	}
	else if (GetAsyncKeyState('L')) {
		cam.AdjustRotation(0, 0.005, 0);
	}

	if (GetAsyncKeyState(' '))
	{
		cam.SetRotation(0, 0, 0);
		cam.SetPosition(0, 0, -2);
	}*/
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return this->cameraPos;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, cameraPos);
	return temp;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return this->rotation_Vector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return this->rotation;
}

void Camera::SetPosition(float x, float y, float z)
{
	cameraPos = XMVectorSet(x, y, z, 0.0f);
	lookAtPos = XMVector2TransformCoord(DEFAULT_FORWARD, rotationMatrix) + cameraPos;
}

void Camera::AdjustPosition(float x, float y, float z)
{
	XMVECTOR temp = XMVectorSet(x, y, z, 0.0f);
	cameraPos = cameraPos + temp;
	lookAtPos = XMVector2TransformCoord(DEFAULT_FORWARD, rotationMatrix) + cameraPos;
}

void Camera::SetRotation(float x, float y)
{
	rotation.x = x;
	rotation.y = y;
	rotation_Vector = XMLoadFloat3(&rotation);
	rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation_Vector);
	lookAtPos = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMatrix) + cameraPos;
	vec_up = XMVector3TransformCoord(DEFAULT_UP, rotationMatrix);
}

const XMVECTOR& Camera::GetForwardVector()
{
	return this->vec_forward;
}

const XMVECTOR& Camera::GetRightVector()
{
	return this->vec_right;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return this->vec_back;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return this->vec_left;
}

void Camera::setLookAtPos(XMFLOAT3 lookAtPos)
{
	lookAtPos.x = this->GetPositionFloat3().x - lookAtPos.x;
	lookAtPos.y = this->GetPositionFloat3().y - lookAtPos.y;
	lookAtPos.z = this->GetPositionFloat3().z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw);
}

bool Camera::createConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	this->immediateContext = immediateContext;

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation_Vector = XMLoadFloat3(&rotation);

	rotationForward.y = 0.0f;
	rotation_Vector_Forward = XMLoadFloat3(&rotationForward);

	vec_forward = DEFAULT_FORWARD;
	vec_up = DEFAULT_UP;
	vec_right = DEFAULT_RIGHT;
	vec_back = DEFAULT_UP;

	rotationForwardMatrix = XMMatrixRotationRollPitchYawFromVector(rotation_Vector_Forward);
	rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation_Vector);
	cameraPos = XMVectorSet(0.0f, 6.0f, 0.0f, 0.0f);
	lookAtPos = XMVectorSet(0.0f, 6.0f, 5.0f, 0.0f);

	worldToView = XMMatrixLookAtLH(cameraPos, lookAtPos, vec_up);
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, vec_up);
	projection = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1024.f / 576, 0.1f, 800.0f);
	viewMatrix *= projection;
	viewMatrix = XMMatrixTranspose(viewMatrix);
	XMStoreFloat4x4(&VP.viewProj, viewMatrix);

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(VP);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&bufferDesc, 0, &ConstantBuffer)))
	{
		ErrorLog::Log("Failed to create Constant buffer for camera");
		return false;
	}

	return true;
}

void Camera::adjustProjectionMatrix(float FOV, float aspectRatio, float nearZ, float farZ)
{
	this->projection = DirectX::XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
}

void Camera::AdjustRotation(float x, float y)
{
	const float max = 1.5f;
	const float min = -1.5f;

	rotation.x += x;
	rotation.y += y;
	rotation_Vector = XMLoadFloat3(&rotation);

	rotationForward.y += y;
	rotation_Vector_Forward = XMLoadFloat3(&rotationForward);

	if (rotation.x > max)
	{
		rotation.x = max;
	}
	else if (rotation.x < min)
	{
		rotation.x = min;
	}

	rotationForwardMatrix = XMMatrixRotationRollPitchYawFromVector(rotation_Vector_Forward);
	rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation_Vector);
	vec_up = XMVector3TransformCoord(DEFAULT_UP, rotationMatrix);
	lookAtPos = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMatrix) + cameraPos;
}

void Camera::sendProjection(int vertexShaderPos)
{
	XMStoreFloat4x4(&VP.viewProj, XMMatrixTranspose(projection));

	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &VP, sizeof(VP));
	immediateContext->Unmap(ConstantBuffer.Get(), 0);
	immediateContext->VSSetConstantBuffers(vertexShaderPos, 1, ConstantBuffer.GetAddressOf());
}

void Camera::sendViewProjection(int vertexShaderPos)
{
	worldToView = XMMatrixLookAtLH(cameraPos, lookAtPos, vec_up);
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, vec_up);
	viewMatrix *= projection;
	viewMatrix = XMMatrixTranspose(viewMatrix);

	XMStoreFloat4x4(&VP.viewProj, viewMatrix);
	D3D11_MAPPED_SUBRESOURCE subData = {};
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &VP, sizeof(VP));
	immediateContext->Unmap(ConstantBuffer.Get(), 0);
	immediateContext->VSSetConstantBuffers(vertexShaderPos, 1, ConstantBuffer.GetAddressOf());
}
