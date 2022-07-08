#include "Camera.h"

Camera::Camera()
{
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->projection = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1024.f / 576, 0.1f, 800.0f);
	this->UpdateViewMatrix();
}

void Camera::moveCamera(Camera& cam, float dt)
{
	if (GetAsyncKeyState('W')) {
		cam.AdjustPosition(cam.GetForwardVector() * dt * 50);
	}
	else if (GetAsyncKeyState('S')) {
		cam.AdjustPosition(cam.GetForwardVector() * dt * -50);
	}

	if (GetAsyncKeyState('A')) {
		cam.AdjustPosition(cam.GetRightVector() * dt * -50);
	}
	else if (GetAsyncKeyState('D')) {
		cam.AdjustPosition(cam.GetRightVector() * dt * 50);
	}

	if (GetAsyncKeyState('K')) {
		cam.AdjustRotation(cam.GetRightVector() * dt * 50);
	}
	else if (GetAsyncKeyState('I')) {
		cam.AdjustRotation(cam.GetRightVector() * dt * -50);
	}

	if (GetAsyncKeyState('J')) {
		cam.AdjustRotation(0, -0.05, 0);
	}
	else if (GetAsyncKeyState('L')) {
		cam.AdjustRotation(0, 0.005, 0);
	}

	if (GetAsyncKeyState('Q')) {
		cam.AdjustPosition(0, -0.1, 0);
	}
	else if (GetAsyncKeyState('E')) {
		cam.AdjustPosition(0, +0.1, 0);
	}

	if (GetAsyncKeyState(' '))
	{
		cam.SetRotation(0, 0, 0);
		cam.SetPosition(0, 0, -2);
	}
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX& Camera::GetProjection() const
{
	return projection;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return this->rot;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(XMVECTOR pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

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

	this->SetRotation(pitch, yaw, 0.0f);
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
	return this->vec_backward;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return this->vec_left;
}

bool Camera::createConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	this->immediateContext = immediateContext;

	XMMATRIX viewProjection = viewMatrix * projection;
	viewProjection = XMMatrixTranspose(viewProjection);
	XMStoreFloat4x4(&VP.viewProj, viewProjection);

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

void Camera::sendViewProjection(Camera& cam, int vertexShaderPos)
{
	XMMATRIX viewProjection = cam.GetViewMatrix() * cam.GetProjection();
	viewProjection = XMMatrixTranspose(viewProjection);

	XMStoreFloat4x4(&VP.viewProj, viewProjection);
	D3D11_MAPPED_SUBRESOURCE subData = {};
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &VP, sizeof(VP));
	immediateContext->Unmap(ConstantBuffer.Get(), 0);
	immediateContext->VSSetConstantBuffers(vertexShaderPos, 1, ConstantBuffer.GetAddressOf());
}

void Camera::UpdateViewMatrix()
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += this->posVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	//Rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}