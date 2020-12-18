#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include"Input.h"

using namespace DirectX;
class Bullet
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�

	//using XMFLOAT3 = DirectX::XMFLOAT3;
	//using XMFLOAT4 = DirectX::XMFLOAT4;
	//using XMMATRIX = DirectX::XMMATRIX;
public:
	Bullet(Object3d *obj, XMFLOAT3& pos, XMFLOAT3& vec);
	Bullet(XMFLOAT3& pos);
	~Bullet();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// ���t���[��
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �x�N�g���ƃx�N�g���̉��Z
	/// </summary>
	/// <param name="position1"></param>
	/// <param name="position2"></param>
	XMFLOAT3 VAdd(const XMFLOAT3& position1, const XMFLOAT3& position2);

	bool BulletSeizon();

	void SetPosition(XMFLOAT3 setpos);

	const XMFLOAT3& GetPosition() { return position; }

private:
	Object3d *bullet = nullptr;
	XMFLOAT3 position;
	Input* input = nullptr;
	Model* model = nullptr;
	bool bulletflag = false;
	bool seizonFlag = false;
	int seizonTime = 0;
public:
	float radius = 10;//�����蔻��p���a

};