#pragma once
#include <DirectXMath.h>

//�J��������N���X
class Camera {

private:

	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;

	//���_�Z�b�g
	static void Seteye(XMFLOAT3 eye);
	static const XMFLOAT3& Geteye() { return eye; }

	//�����_�Z�b�g
	static void SetTarget(XMFLOAT3 target);
	static const XMFLOAT3& GetTarget() { return target; }



	//�r���[�s��X�V
	static void UpdateView();

public:
	//�������ݒ�
	static void InitializeCamera(int window_width, int window_height);
	//�J�����ړ�(���_&�����_)
	static void CameraMove(XMFLOAT3 move);
	//�J���������_���v���C���[��
	static void PlayerTarget(XMFLOAT3 playerpos);
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
};