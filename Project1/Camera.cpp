#include "Camera.h"
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

XMFLOAT3 Camera::eye = { 0, 0, -50.0f };
XMFLOAT3 Camera::target = { 0, 0, 0 };
XMFLOAT3 Camera::up = { 0, 1, 0 };
XMMATRIX Camera::matView{};
XMMATRIX Camera::matProjection{};

void Camera::InitializeCamera(int window_width, int window_height)
{
	// ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 10000.0f
	);
}


void Camera::Seteye(XMFLOAT3 eye)
{
	Camera::eye = eye;
	UpdateView();
}

void Camera::SetTarget(XMFLOAT3 target)
{
	Camera::target = target;
	UpdateView();
}

void Camera::CameraMove(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = Geteye();
	//XMFLOAT3 target_moved = GetTarget();
	
	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	//target_moved.x += move.x;
	//target_moved.y += move.y;
	//target_moved.z += move.z;

	Seteye(eye_moved);
	//SetTarget(target_moved);
}

void Camera::PlayerTarget(XMFLOAT3 playerpos)
{
	XMFLOAT3 eyepos = Geteye();
	eyepos.x = playerpos.x;
	eyepos.y = playerpos.y + 10;
	eyepos.z = playerpos.z - 30;

	Seteye(eyepos);

	//注視点をプレイヤーの座標に
	SetTarget(playerpos);
}

void Camera::UpdateView()
{
	// ビュー行列の更新
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}
