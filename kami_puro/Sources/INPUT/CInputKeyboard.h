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
	KEYBOARD_CODE_DECIDE = 0,
	KEYBOARD_CODE_UI_START_TEST,
	KEYBOARD_CODE_UI_UP_CROWD_RIGHT_TEST,
	KEYBOARD_CODE_UI_UP_CROWD_LEFT_TEST,
	KEYBOARD_CODE_UI_UP_HP_TEST,
	KEYBOARD_CODE_UI_DOWN_HP_TEST,
	KEYBOARD_CODE_UI_COMMAND_ANIME_CLOSE,	// �R�}���h�`���[�g�̃A�j���[�V������ON
	KEYBOARD_CODE_UI_COMMAND_ANIME_OPEN,	// �R�}���h�`���[�g�̃A�j���[�V������OFF
	KEYBOARD_CODE_UI_CUT_IN_0,				// �J�b�g�C�� ���̃v���C��
	KEYBOARD_CODE_UI_CUT_IN_1,				// �J�b�g�C�� �E�̃v���C��
	KEYBOARD_CODE_UI_FADE,					// �t�F�[�h��UI
	KEYBOARD_CODE_CAMERA_ROT_UP,
	KEYBOARD_CODE_CAMERA_ROT_DOWN,
	KEYBOARD_CODE_CAMERA_ROT_LEFT,
	KEYBOARD_CODE_CAMERA_ROT_RIGHT,
	KEYBOARD_CODE_CAMERA_SMALL_SHAKE,
	KEYBOARD_CODE_CAMERA_BIG_SHAKE,
	KEYBOARD_CODE_CAMERA_MOVE,
	KEYBOARD_CODE_CAMERA_SET,
	KEYBOARD_CODE_PLAYER_1_LEFT_UP,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_1_LEFT_DOWN,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_1_RIGHT_UP,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_2_LEFT_UP,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_2_LEFT_DOWN,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_2_RIGHT_UP,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN,	// �R�}���h�`���[�g�̃f�o�b�O�p
	KEYBOARD_CODE_CAMERA_SET1,
	KEYBOARD_CODE_CAMERA_SET2,
	KEYBOARD_CODE_CAMERA_SET3,
	KEYBOARD_CODE_CAMERA_MOVE1,
	KEYBOARD_CODE_CAMERA_MOVE2,
	KEYBOARD_CODE_ROPE_BIYON,			// ���[�v���r�������Ă��� SPACE�L�[
	KEYBOARD_CODE_FORCE_BATTLE_MODE,
	KEYBOARD_CODE_FORCE_MOVE_MODE,
	KEYBOARD_CODE_PLAYER_1_Z,
	KEYBOARD_CODE_PLAYER_1_X,
	KEYBOARD_CODE_PLAYER_1_E,
	KEYBOARD_CODE_PLAYER_1_D,
	KEYBOARD_CODE_PLAYER_1_C,
	KEYBOARD_CODE_PLAYER_2_COMMA,
	KEYBOARD_CODE_PLAYER_2_PERIOD,
	KEYBOARD_CODE_PLAYER_2_P,
	KEYBOARD_CODE_PLAYER_2_SEMICOLON,
	KEYBOARD_CODE_PLAYER_2_SLASH,
	KEYBOARD_CODE_UI_OVERLAY,			// �I�[�o�[���C(Ready�Ƃ��̕\��)
	KEYBOARD_CODE_FINISH_FLAG_REVERSAL,	// FINISH�Z�g�p�\�t���O���]
	KEYBOARD_CODE_UI_ROPE_TIMER,		// ���[�v�^�C�}�[
	KEYBOARD_CODE_UI_STOP_TIMER,		// ���v���~�߂�
	KEYBOARD_CODE_UI_FORCUS_LINE,		// �W�������o��

	KEYBOARD_CODE_MAX
}KEYBOARD_CODE;

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
	static	bool GetKeyboardPress(KEYBOARD_CODE key);

	//======================================
	// �L�[�{�[�h�̃g���K�[�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardTrigger(KEYBOARD_CODE key);

	//======================================
	// �L�[�{�[�h�̃��s�[�g�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardRepeat(KEYBOARD_CODE key);

	//======================================
	// �L�[�{�[�h�̃����[�X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static	bool GetKeyboardRelease(KEYBOARD_CODE key);

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