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
#include "CUiManager.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "../CGame.h"
#include "../COMMANDCHART/CCommandChartManager.h"
#include "CCutIn.h"
#include "CBattleFade.h"
#include "../../../MANAGER/CManager.h"
#include "COverLay.h"
#include "CRopeTimer.h"
#include "CForcusLine.h"
#include "../../../SOUND/CSound.h"
//*****************************************************************************
// �萔
//*****************************************************************************
// �����Q�[�W�̍���
static const float CROWD_HEIGHT = 15;
// �����Q�[�W��Y���W
static const float CROWD_POS_Y = 75;
// �����Q�[�W�̍��̍��W
static const float CROWD_POS_LEFT_X = 225;
// �����Q�[�W�̉E�̍��W
static const float CROWD_POS_RIGHT_X = SCREEN_WIDTH - 225;

// HP�Q�[�W�̍����ƕ�
static const float HP_HEIGHT = 34;
// HP�Q�[�W��Y���W
static const float HP_POS_Y = 121;
// HP�Q�[�W�̍��T�C�h��X���W ���_�ɋ߂��ق���START
static const float HP_POS_X_LEFT_START = SCREEN_WIDTH * 0.5f - 50;
static const float HP_POS_X_LEFT_END = 120;
// HP�Q�[�W�̉E�T�C�h��X���W ���_�ɋ߂��ق���START
static const float HP_POS_X_RIGHT_START = SCREEN_WIDTH * 0.5f + 50;
static const float HP_POS_X_RIGHT_END = SCREEN_WIDTH - 120;

// �Q�[���̃J�E���g�^�C��
static const int COUNT_TIMER_MAX = 99;
// �Q�[���̃J�E���g�^�C���̍��W
static const D3DXVECTOR2 COUNT_TIME_POS = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 120);

//=============================================================================
// �R���X�g���N�^
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pDevice = pDevice;
	m_pStaminaBarL = NULL;
	m_pStaminaBarR = NULL;
	m_pCrowdBar = NULL;
	m_pHpBar = NULL;
	m_pTimer = NULL;
	m_pManager = pManager;
	m_pCommandChartManager = NULL;
	m_pCutIn = NULL;
	m_pBattleFade = NULL;
	m_pRopeTimer = NULL;
	m_pForcusLine = NULL;
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
void CUiManager::Init(CGame *pGame)
{
	m_pGame = pGame;
	
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
	
	// �������Ԃ̕\��
	m_pTimer = CCountTime::Create(
		D3DXVECTOR2(COUNT_TIME_POS),
		COUNT_TIMER_MAX,
		m_pDevice,
		m_pManager,
		m_pGame);

	// �R�}���h�`���[�g�}�l�[�W���[�̍쐬
	m_pCommandChartManager = CCommandChartManager::Create(m_pDevice, m_pManager->GetPlayerManager());
	// �J�b�g�C��
	m_pCutIn = CCutIn::Create(m_pDevice);
	// �o�g���t�F�[�h
	m_pBattleFade = CBattleFade::Create(m_pDevice);
	// �I�[�o�[���C
	m_pOverLay = COverLay::Create(m_pDevice);
	// ���[�v�^�C�}�[
	m_pRopeTimer = CRopeTimer::Create(m_pDevice);
	// �W����
	m_pForcusLine = CForcusLine::Create(m_pDevice);
}

//=============================================================================
// �I��
//=============================================================================
void CUiManager::Uninit(void)
{
	// ������̓C���X�^���X�������Ă邾��������delete���K�v
	m_pForcusLine->Uninit();
	m_pRopeTimer->Uninit();
	m_pOverLay->Uninit();
	m_pBattleFade->Uninit();
	m_pCutIn->Uninit();
	m_pCrowdBar->Uninit();
	m_pTimer->Uninit();
	m_pHpBar->Uninit();
	m_pCommandChartManager->Uninit();

	SAFE_DELETE(m_pForcusLine);
	SAFE_DELETE(m_pRopeTimer);
	SAFE_DELETE(m_pOverLay);
	SAFE_DELETE(m_pBattleFade);
	SAFE_DELETE(m_pCutIn);
	SAFE_DELETE(m_pCrowdBar);
	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pHpBar);
	SAFE_DELETE(m_pCommandChartManager);
}

//=============================================================================
// �X�V
//=============================================================================
void CUiManager::Update(void)
{
	// �eUI�̍X�V
	m_pRopeTimer->Update();
	m_pOverLay->Update();
	m_pCrowdBar->Update();
	m_pTimer->Update();
	m_pHpBar->Update();
	m_pCutIn->Update();
	m_pBattleFade->Update();
	m_pForcusLine->Update();
	// �R�}���h�`���[�g�̍X�V
	m_pCommandChartManager->Update();

#ifdef _DEBUG
	// �ϋq�Q�[�W�̑���
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_RIGHT_TEST))
	{
		m_pCrowdBar->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_LEFT_TEST))
	{
		m_pCrowdBar->Add(-20);
	}
	// HP�̑���
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_HP_TEST))
	{
		m_pHpBar->AddLeft(5);
		m_pHpBar->AddRight(5);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_DOWN_HP_TEST))
	{
		m_pHpBar->SubLeft(5);
		m_pHpBar->SubRight(5);
	}
	// �J�b�g�C���̃e�X�g
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_CUT_IN_0))
	{
		m_pCutIn->Start(0, CUT_IN_JIJII);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_CUT_IN_1))
	{
		m_pCutIn->Start(1, CUT_IN_JIJII);
	}
	// �V�[���؂�ւ��Ƃ��̃t�F�[�h
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_FADE))
	{
		m_pBattleFade->Start(BATTLE_FADE_LIGHT);
	}
	// �I�[�o�[���C(Ready�Ƃ�)
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_OVERLAY))
	{
		m_pOverLay->Start(&COverLay::Data(TEXTURE_ROPE, 0.1f, 30, 0.1f));
	}
	// ���[�v�^�C�}�[
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_ROPE_TIMER))
	{
		m_pRopeTimer->Start(30 ,80);
	}
	// �W��������
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_FORCUS_LINE))
	{
		m_pForcusLine->Start(60, false);
	}
#endif
}

/*
//=============================================================================
// �쐬
//=============================================================================
CUiManager* CUiManager::Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame *pGame)
{
	CUiManager* p = new CUiManager(pDevice, pManager);
	p->Init(pGame);
	return p;
}
*/
//=============================================================================
// �Q�[���J�n�̃A�j���[�V����������֐�
//=============================================================================
void CUiManager::StartAnimation(int interval)
{
	// �eUI�A�j���[�V�������J�n
	m_pTimer->StartAnimation(interval);
	m_pHpBar->StartAnimation(interval);
	m_pCrowdBar->StartAnimation(interval);
}

//=============================================================================
// �o�g���t�F�[�h�X�^�[�g
//=============================================================================
void CUiManager::StartBattleFade(void)
{ 
	m_pBattleFade->Start(BATTLE_FADE_LIGHT); 
	m_pManager->PlaySoundA(SOUND_LABEL_SE_BATTLE_FADE);
}

//=============================================================================
// �W�������o��
//=============================================================================
void CUiManager::StartForcusLine(int timerFrameCount, bool isFinish)
{
	m_pForcusLine->Start(timerFrameCount, isFinish);
}

//=============================================================================
// �I�[�o�[���C�X�^�[�g
//=============================================================================
void CUiManager::StartOverLay(COverLay::Data *data)
{
	m_pOverLay->Start(data);
}

//=============================================================================
// �\��
//=============================================================================
void CUiManager::SetImvisible(void)
{
	m_pHpBar->SetImvisible();
	m_pCrowdBar->SetImvisible();
	m_pTimer->SetImvisible();
}

//=============================================================================
// ��\��
//=============================================================================
void CUiManager::SetVisible(void)
{
	m_pCrowdBar->SetVisible();
	m_pTimer->SetVisible();
	m_pHpBar->SetVisible();
}
//----EOF----