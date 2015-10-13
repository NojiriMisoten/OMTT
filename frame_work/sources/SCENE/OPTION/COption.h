//=============================================================================
//
// COption�N���X [COption.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _COPTION_H_
#define _COPTION_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../CONTROLLER/CControllerManager.h"
#include "../../INPUT/CInputGamePad.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CSceneNumber;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class COption : public CPhase
{
public:
	// �R���X�g���N�^
	COption(void);

	// �f�X�g���N�^
	~COption(void);

	//=================================================================
	// �������֐�
	// ����: �V�[���̃��[�h, �f�o�C�X
	//=================================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//=================================================================
	// �N���G�C�g�֐�
	// ����: �V�[���̃��[�h, �f�o�C�X
	// �߂�l: ���̃V�[���̃A�h���X
	//=================================================================
	static COption* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:
	// ���[�h
	typedef enum
	{
		OPTION_MODE_KEYCONFIG,
		OPTION_MODE_NEXT_SCENE,
		OPTION_MODE_MAX
	}OPTION_MODE;

	//=================================================================
	// �L�[�R���t�B�O�֐�
	//=================================================================
	void KeyConfig(int padID, CInputGamePad::PAD_BUTTON button);

	//=================================================================
	// �����o�֐�
	// ����:(out)�P�O�̈ʁA(out)�P�̈ʁA���̐���
	//=================================================================
	void CuclDigit(int* ten, int* one, int org);

	//=================================================================
	// �J�[�\���X�V�֐�
	//=================================================================
	void UpdateCursol(void);

	// �����w�i�̐�
	static const int MOVE_BG_NUM = 2;

	// �L�[�R�[�h�\���p
	typedef struct
	{
		CSceneNumber* DigitTen;	// 10�̈�
		CSceneNumber* DigitOne;	// 1�̈�
	}PRINT_KEY_CORD;

	OPTION_MODE					m_mode;
	CInputGamePad::PAD_BUTTON	m_setButton;											// ���̃{�^����
	int							m_nControllerID;										// �R���g���[���[�ԍ�
	CScene2D*					m_pOptionMenu[CControllerManager::MAX_CONTROLLER_NUM];	// �I�v�V�������j���[
	CScene2D*					m_pOptionBG;											// �I�v�V����BG
	CScene2D*					m_pOptionBGMove[MOVE_BG_NUM];							// �I�v�V��������BG
	PRINT_KEY_CORD				m_KeyNumber[CControllerManager::MAX_CONTROLLER_NUM][CInputGamePad::PAD_KEY_MAX];
	CScene2D*					m_pCursol[CControllerManager::MAX_CONTROLLER_NUM];		// �J�[�\��
	float						m_fCursolAlfaRad;										// �J�[�\���̓����l
	int							m_nSelectCounter[CControllerManager::MAX_CONTROLLER_NUM];// �ǂ���I��ł��邩
	CScene2D*					m_pReturn;												// �߂�
};

#endif
//----EOF----