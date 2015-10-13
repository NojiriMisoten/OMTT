//=============================================================================
//
// CInputKeyboard�N���X [CInputKeyboard.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CINPUTKEYBOARD_H_
#define _CINPUTKEYBOARD_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInput.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInputKeyboard : CInput
{
public:
	// �R���X�g���N�^
	CInputKeyboard(void);

	// �f�X�g���N�^
	~CInputKeyboard(void);

	//============================================
	// ������
	// ����: �C���X�^���X�A�E�C���h�E�n���h��
	// �߂�l: ����
	//============================================
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//======================================
	// �L�[�{�[�h�̃v���X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardPress(BYTE key);

	//======================================
	// �L�[�{�[�h�̃g���K�[�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardTrigger(BYTE key);

	//======================================
	// �L�[�{�[�h�̃��s�[�g�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardRepeat(BYTE key);

	//======================================
	// �L�[�{�[�h�̃����[�X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardRelease(BYTE key);

private:
	static const int KEY_NUM = 256;								//�L�[�̐�
	static const int REPEAT_TIME = 30;							//���s�[�g�L�[���������鎞��
	
	static	BYTE	m_aKeyState[KEY_NUM];						//�L�[�{�[�h�̉�����Ԃ��󂯎�郏�[�N
	static	BYTE	m_TrigKeyState[KEY_NUM];					//�g���K�[�L�[�p
	static	BYTE	m_ReleaseKeyState[KEY_NUM];					//�����[�X�L�[�p
	static	BYTE	m_RepeatKeyState[KEY_NUM];					//���s�[�g�p
	static	int		m_RepeatKeyCount[KEY_NUM];					//���s�[�g�L�[�p
	static	bool	m_Repeatflag[KEY_NUM];						//���s�[�g�L�[�p�t���O
};

#endif
//----EOF----