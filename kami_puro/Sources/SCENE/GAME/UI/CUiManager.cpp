//=============================================================================
//
// CUiManager�N���X [CUiManager.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCountTime.h"
#include "CCrowdBar.h"
#include "CHpBar.h"
#include "CStaminaBar.h"
#include "CUiManager.h"
#include "CFace.h"
#include "../../../INPUT/CInputKeyboard.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �獶�T�C�h�̍��W
static const D3DXVECTOR2 FACE_POS_LEFT = D3DXVECTOR2(90, 140);
// ��E�T�C�h�̍��W
static const D3DXVECTOR2 FACE_POS_RIGHT = D3DXVECTOR2(SCREEN_WIDTH - 90, 140);
// ��̑傫��
static const D3DXVECTOR2 FACE_SIZE = D3DXVECTOR2(100, 100);

// �����Q�[�W�̍���
static const float CROWD_HEIGHT = 20;
// �����Q�[�W��Y���W
static const float CROWD_POS_Y = 75;
// �����Q�[�W�̍��̍��W
static const float CROWD_POS_LEFT_X = 170;
// �����Q�[�W�̉E�̍��W
static const float CROWD_POS_RIGHT_X = SCREEN_WIDTH - 170;

// HP�Q�[�W�̍����ƕ�
static const float HP_HEIGHT = 30;
// HP�Q�[�W��Y���W
static const float HP_POS_Y = 125;
// HP�Q�[�W�̍��T�C�h��X���W ���_�ɋ߂��ق���START
static const float HP_POS_X_LEFT_START = SCREEN_WIDTH * 0.5f - 50;
static const float HP_POS_X_LEFT_END = 100;
// HP�Q�[�W�̉E�T�C�h��X���W ���_�ɋ߂��ق���START
static const float HP_POS_X_RIGHT_START = SCREEN_WIDTH * 0.5f + 50;
static const float HP_POS_X_RIGHT_END = SCREEN_WIDTH - 100;

// �Q�[���̃J�E���g�^�C��
static const int COUNT_TIMER_MAX = 99;
// �Q�[���̃J�E���g�^�C���̍��W
static const D3DXVECTOR2 COUNT_TIME_POS = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 125);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_pStaminaBarL = NULL;
	m_pStaminaBarR = NULL;
	m_pCrowdBar = NULL;
	m_pHpBar = NULL;
	m_pFace = NULL;
	m_pTimer = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUiManager::~CUiManager(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CUiManager::Init()
{
	// ���܃R���`�悵�ĂȂ�
	m_pStaminaBarL = CStaminaBar::Create(
		D3DXVECTOR2(50, 100),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f - 50, 100),
		CStaminaBar::POSITIONBASE_LEFT, m_pDevice);
	m_pStaminaBarR = CStaminaBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f + 50, 100),
		D3DXVECTOR2(SCREEN_WIDTH - 50, 100),
		CStaminaBar::POSITIONBASE_RIGHT, m_pDevice);
	
	// HP
	m_pHpBar = CHpBar::Create(
		HP_HEIGHT,
		HP_POS_Y,
		HP_POS_X_LEFT_END, HP_POS_X_LEFT_START,
		HP_POS_X_RIGHT_START, HP_POS_X_RIGHT_END,
		m_pDevice);
	
	// �ϐ�
	m_pCrowdBar = CCrowdBar::Create(
		CROWD_HEIGHT, CROWD_POS_Y,
		CROWD_POS_LEFT_X, CROWD_POS_RIGHT_X,
		m_pDevice);
	
	// ��
	m_pFace = CFace::Create(
		D3DXVECTOR2(FACE_POS_LEFT), D3DXVECTOR2(FACE_POS_RIGHT),
		D3DXVECTOR2(FACE_SIZE), m_pDevice);
	
	// �������Ԃ̕\��
	m_pTimer = CCountTime::Create(D3DXVECTOR2(COUNT_TIME_POS), COUNT_TIMER_MAX, m_pDevice);

}

//=============================================================================
// �I��
//=============================================================================
void CUiManager::Uninit(void)
{
	// ������̓C���X�^���X�������Ă邾��������delete���K�v
	m_pCrowdBar->Uninit();
	m_pTimer->Uninit();
	m_pFace->Uninit();
	m_pHpBar->Uninit();

	SAFE_DELETE(m_pCrowdBar);
	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pFace);
	SAFE_DELETE(m_pHpBar);
}

//=============================================================================
// �X�V
//=============================================================================
void CUiManager::Update(void)
{
	// �X�V
	m_pCrowdBar->Update();
	m_pTimer->Update();
	m_pFace->Update();
	m_pHpBar->Update();

	// test
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_USE_STAMINA_TEST))
	{
		m_pStaminaBarL->UseStamina(50);
		m_pStaminaBarR->UseStamina(50);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_RIGHT_TEST))
	{
		m_pCrowdBar->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_LEFT_TEST))
	{
		m_pCrowdBar->Add(-20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_HP_TEST))
	{
		m_pHpBar->AddLeft(20);
		m_pHpBar->AddRight(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_DOWN_HP_TEST))
	{
		m_pHpBar->SubLeft(20);
		m_pHpBar->SubRight(20);
	}
}

//=============================================================================
// �쐬
//=============================================================================
CUiManager* CUiManager::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	CUiManager* p = new CUiManager(pDevice);
	p->Init();
	return p;
}

//=============================================================================
// �Q�[���J�n�̃A�j���[�V����������֐�
//=============================================================================
void CUiManager::StartAnimation(int interval)
{
	// �eUI�A�j���[�V�������J�n
	m_pTimer->StartAnimation(interval);
	m_pFace->StartAnimation(interval);
	m_pHpBar->StartAnimation(interval);
	m_pCrowdBar->StartAnimation(interval);
}

//----EOF----