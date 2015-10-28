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
// �L�[�{�[�h�̓��͗񋓑�
//*****************************************************************************
typedef enum
{
	KEYBOARD_CORD_DECIDE = 0,
	KEYBOARD_CORD_PLAYER_1_FORWORD,
	KEYBOARD_CORD_PLAYER_1_BACK,
	KEYBOARD_CORD_PLAYER_1_ROT_RIGHT,
	KEYBOARD_CORD_PLAYER_1_ROT_LEFT,
	KEYBOARD_CORD_UI_START_TEST,
	KEYBOARD_CORD_UI_USE_STAMINA_TEST,
	KEYBOARD_CORD_UI_UP_CROWD_RIGHT_TEST,
	KEYBOARD_CORD_UI_UP_CROWD_LEFT_TEST,
	KEYBOARD_CORD_UI_UP_HP_TEST,
	KEYBOARD_CORD_UI_DOWN_HP_TEST,
	KEYBOARD_CORD_CAMERA_ROT_UP,
	KEYBOARD_CORD_CAMERA_ROT_DOWN,
	KEYBOARD_CORD_CAMERA_ROT_LEFT,
	KEYBOARD_CORD_CAMERA_ROT_RIGHT,
	KEYBOARD_CORD_CAMERA_SMALL_SHAKE,
	KEYBOARD_CORD_CAMERA_BIG_SHAKE,
	KEYBOARD_CORD_CAMERA_MOVE,
	KEYBOARD_CORD_CAMERA_SET,
	KEYBOARD_CORD_COMMAND_DEBUG_Q,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CORD_COMMAND_DEBUG_W,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CORD_COMMAND_DEBUG_A,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CORD_COMMAND_DEBUG_S,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CORD_MAX
}KEYBOARD_CORD;

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
	static	bool GetKeyboardPress(KEYBOARD_CORD key);

	//======================================
	// �L�[�{�[�h�̃g���K�[�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardTrigger(KEYBOARD_CORD key);

	//======================================
	// �L�[�{�[�h�̃��s�[�g�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardRepeat(KEYBOARD_CORD key);

	//======================================
	// �L�[�{�[�h�̃����[�X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardRelease(KEYBOARD_CORD key);

private:
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