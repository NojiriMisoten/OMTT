//=============================================================================
//
// CInputGamePad�N���X [CInputGamePad.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CINPUTGAMEPAD_H_
#define _CINPUTGAMEPAD_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInput.h"

//**********************************************
// �}�N��
//**********************************************
// �p�b�h�̕����L�[�p
#define PP_NONE 0x0
#define PP_UP 0x1
#define PP_RIGHT 0x2
#define PP_DOWN 0x4
#define PP_LEFT 0x8


//**********************************************
// �\���̒�`
//**********************************************

// �f�o�C�X�񋓊֐��ɓn���f�[�^���i�[����\����
typedef struct
{
	LPDIRECTINPUT8 pInput;				// �f�o�C�X���쐬���邽�߂̃C���^�[�t�F�C�X
	LPDIRECTINPUTDEVICE8 *ppPadDevice;	// �g�p����f�o�C�X���i�[����|�C���^�̃|�C���^
	int padID;
}STR_ENUMDATA;

// �R���g���[���[�̍\����
typedef struct
{
	LPDIRECTINPUTDEVICE8	pPadDevice;				// �f�o�C�X
	GUID					Controller_ID;			// �R���g���[���[�̂h�c
	DIJOYSTATE2				PadData;				// ����̃p�b�h�̃f�[�^
	DIJOYSTATE2				OldPadData;				// �O��̃p�b�h�̃f�[�^
	BYTE					DirectryData;			// ����̕����f�[�^
	BYTE					OldDirectryData;		// �O��̕����f�[�^
}GAMEPAD;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInputGamePad : CInput
{
public:
	// ���z�R���g���[���[�{�^���̎��
	typedef enum
	{
		LEFT_STICK_LEFT = 0,	// ��
		LEFT_STICK_RIGHT,		// ��
		LEFT_STICK_UP,			// ��
		LEFT_STICK_DOWN,		// ��
		LEFT_STICK_LEFT_UP,		// ����
		LEFT_STICK_LEFT_DOWN,	// ����
		LEFT_STICK_RIGHT_UP,	// ����
		LEFT_STICK_RIGHT_DOWN,	// ����
		RIGHT_STICK_LEFT,		// ��
		RIGHT_STICK_RIGHT,		// ��
		RIGHT_STICK_UP,			// ��
		RIGHT_STICK_DOWN,		// ��
		KEY_A,					// A
		KEY_B,					// B
		KEY_Y,					// Y
		KEY_X,					// X
		KEY_L,					// L
		KEY_R,					// R
		KEY_START,				// Start
		KEY_SELECT,				// Select
		KEY_DECIDE,				// ����
		KEY_MAX
	}KEY;

	// �L�[�̎��(�L�[�R�t�B���O�ł�����)
	typedef enum
	{
		PAD_KEY_CHANGE_NORMAL = 0,
		PAD_KEY_CHANGE_ATK,
		PAD_KEY_CHANGE_SPD,
		PAD_KEY_CHANGE_JAMMER,
		PAD_KEY_THREAD,
		PAD_KEY_ATTACK,
		PAD_KEY_START,
		PAD_KEY_SELECT,
		PAD_KEY_MAX
	}PAD_BUTTON;

	// �R���X�g���N�^
	CInputGamePad(void);

	// �f�X�g���N�^
	~CInputGamePad(void);

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
	
	// �R�[���o�b�N�֐�
	static BOOL CALLBACK EnumGamePad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext);
	static BOOL CALLBACK EnumObject(LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef);

	//======================================
	// �W���C�p�b�h�̃v���X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static bool GetGamePadPress(KEY key, int playerID);

	//======================================
	// �W���C�p�b�h�̃g���K�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	static bool GetGamePadTrigger(KEY key, int playerID);

	//======================================
	// �W���C�p�b�h�̃����[�X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	static bool GetGamePadRelease(KEY key, int playerID);

	//======================================
	// �W���C�p�b�h�̃��s�[�g�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ��莞�ԉ�������ςȂ���
	//======================================
	static bool GetGamePadRepeat(KEY key, int playerID);

	//======================================
	// �f���p�̃L�[���͕ۑ�
	// ����: ���t���[���ڂ�
	//======================================
	static void KeepKeyForDemo(int frame);

	//======================================
	// �f���p�̃L�[���͕ۑ�
	// ����: �������L�[�A���t���[���ڂ�
	//======================================
	static void SaveKeyForDemo(KEY key, int idx);

	// �f���p�̃L�[���̓t�@�C���o��
	static void OutPutKeyForDemo(void);

	// �f���p�̃L�[�ǂݍ���
	static void LoadKeyForDemo(void);

	//======================================
	// �f���p�̃L�[�擾
	// ����: �������L�[�A���t���[���ڂ�
	//======================================
	static bool GetKeyForDemo(KEY key, int idx);

	//======================================
	// �f���p�̃g���K�[�L�[�擾
	// ����: �������L�[�A���t���[���ڂ�
	//======================================
	static bool GetTriggerKeyForDemo(KEY key, int idx);
	
	//======================================
	// �f���p�̃����[�X�L�[�擾
	// ����: �������L�[�A���t���[���ڂ�
	//======================================
	static bool GetReleaseKeyForDemo(KEY key, int idx);

	//======================================
	// �L�[�R�t�B���O���Z�b�g
	// ����: �R���g���[���[ID, �L�[�R�t�B���O���̔z��A�h���X
	//======================================
	static void SetKeyCofingInfo(int id, int* keyConfigInfo);

	//======================================
	// �����g���K�[������
	// ����: �R���g���[���[ID, �����ꂽ�L�[�R�[�h�ۑ�����ꏊ
	// �߂�l�F�g���K�[����Ă���
	//======================================
	static bool CheckTriggerAnyKey(int ID, int* pCord);

	//======================================
	// �R���g���[���[�q�����Ă邩���ׂ�
	// ����: �R���g���[���[ID
	// �߂�l�F�q�����Ă邩
	//======================================
	static bool CheckConectPad(int ID);

private:
	// �f���p�L�[�̕ۑ��ꏊ�}�b�v
	typedef enum
	{
		L_UP_KEY	= 0x0001,
		L_DOWN_KEY	= 0x0002,
		L_LEFT_KEY	= 0x0004,
		L_RIGHT_KEY = 0x0008,
		JUMP_KEY	= 0x0010,
		KICK_KEY	= 0x0020,
		R_UP_KEY	= 0x0040,
		R_DOWN_KEY	= 0x0080,
		R_LEFT_KEY	= 0x0100,
		R_RIGHT_KEY = 0x0200,
		R_STICK_KEY = 0x0400,
	}KEEP_KEY;

	// �W���C�p�b�h�̃L�[���ݒ�
	static void SetKeyStateGamePad(void);

	static const int INPUT_DEEP_X = 15;						// �A�i���O�X�e�B�b�N�̎󂯎��͈�
	static const int INPUT_DEEP_Y = 290;					// �A�i���O�X�e�B�b�N�̎󂯎��͈�
	static const int MAX_KEEP_KEY_FOR_DEMO = 1800;			// �f���̍ő�t���[����
	static const int m_kInputDeep				= 8;		// ���͐[�x
	static const int m_KeyStateMask				= 0x0080;	// �L�[��ԃ}�X�N�p�萔
	static const int m_KeyMaxNum				= 256;		// �L�[�̑���
	static const int m_KeyRepeatInputStartTime	= 30;		// ���s�[�g�L�[�̓��͊J�n����
	static const int m_KeyRepeateIntervalTime	= 1;		// ���s�[�g�L�[�Ԋu�̎���
	static const int KEY_MAX_NUM = 256;						// �L�[�̑���
	static const int MAX_PAD_NUM = 4;						// �R���g���[���[�̍ő吔

	static GAMEPAD	m_GamePad[MAX_PAD_NUM];									// �Q�[���p�b�h�̏��
	static GUID		m_pad_discrimination[MAX_PAD_NUM];						// �e�f�o�C�X�̎��ʎq���i�[
	static bool		m_aKeyStateGamePad[MAX_PAD_NUM][KEY_MAX_NUM];			// �W���C�p�b�h�̉�����Ԃ�ێ����郏�[�N
	static bool		m_aKeyStateTriggerGamePad[MAX_PAD_NUM][KEY_MAX_NUM];	// �W���C�p�b�h�̃g���K�[��Ԃ�ێ����郏�[�N
	static bool		m_aKeyStateReleaseGamePad[MAX_PAD_NUM][KEY_MAX_NUM];	// �W���C�p�b�h�̃����[�X��Ԃ�ێ����郏�[�N
	static bool		m_aKeyStateRepeatGamePad[MAX_PAD_NUM][KEY_MAX_NUM];		// �W���C�p�b�h�̃��s�[�g��Ԃ�ێ����郏�[�N
	static int		m_aKeyStateRepeatCntGamePad[MAX_PAD_NUM][KEY_MAX_NUM];	// �W���C�p�b�h�̃��s�[�g�J�E���^
	static unsigned short	m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];			// �f���p�̃L�[�̃��[�N
	static int				m_nKeyFrameForDemo;								// �f���p�̃L�[�t���[��
	
	static int		m_aKeyConfig[MAX_PAD_NUM][PAD_KEY_MAX];	// ���ꂼ��̃L�[�R���t�B�O���
};

#endif
//----EOF----