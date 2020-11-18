#pragma once
#include <Windows.h>

/// <summary>
/// �E�B���h�E�Y�A�v���P�[�V����
/// </summary>
class WinApp
{
public: // �ÓI�����o�ϐ�
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;	// ����
	static const int window_height = 720;	// �c��
	static const wchar_t windowClassName[];

public: // �ÓI�����o�֐�
	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <param name="msg">���b�Z�[�W�ԍ�</param>
	/// <param name="wparam">���b�Z�[�W���1</param>
	/// <param name="lparam">���b�Z�[�W���2</param>
	/// <returns>����</returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �����o�֐�
	/// <summary>
	/// �Q�[���E�B���h�E�̍쐬
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// �Q�[���E�B���h�E�̔j��
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	/// <returns>�I�����ǂ���</returns>
	bool ProcessMessage();

	/// <summary>
	/// �E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd; }

	HINSTANCE GetInstance() { return wndClass.hInstance; }

private: // �����o�ϐ�
	// Window�֘A
	HWND hwnd = nullptr;	// �E�B���h�E�n���h��
	WNDCLASSEX wndClass{};			// �E�B���h�E�N���X
};

