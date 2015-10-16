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
static const D3DXVECTOR2 FACE_POS_LEFT = D3DXVECTOR2(50, 50);
// ��E�T�C�h�̍��W
static const D3DXVECTOR2 FACE_POS_RIGHT = D3DXVECTOR2(SCREEN_WIDTH - 50, 50);
// �獶�T�C�h�̑傫��
static const D3DXVECTOR2 FACE_SIZE_LEFT = D3DXVECTOR2(50, 50);
// ��E�T�C�h�̑傫��
static const D3DXVECTOR2 FACE_SIZE_RIGHT = D3DXVECTOR2(50, 50);

// �����Q�[�W�̍���
static const float CROWD_HEIGHT = 30;

// HP�Q�[�W�̍����ƕ�
static const float HP_WIDTH = 300;
static const float HP_HEIGHT = 30;
// HP�Q�[�W�̒��S����̋���
static const float HP_CENTER_DIST_X = 30;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_pStaminaBarL = NULL;
	m_pStaminaBarR = NULL;
	m_pCrowdBar = NULL;
	m_pHpBarL = NULL;
	m_pHpBarR = NULL;
	m_pFace = NULL;
	m_pTimer = NULL;
	m_isAnimation = false;
	m_AnimationCount = 0;
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
	m_pHpBarL = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_LEFT,
		m_pDevice);
	m_pHpBarR = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_RIGHT,
		m_pDevice);
	
	// �ϐ�
	m_pCrowdBar = CCrowdBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 50),
		CROWD_HEIGHT,
		m_pDevice);
	
	// ��
	m_pFace = CFace::Create(
		D3DXVECTOR2(FACE_POS_LEFT), D3DXVECTOR2(FACE_SIZE_LEFT),
		D3DXVECTOR2(FACE_POS_RIGHT), D3DXVECTOR2(FACE_SIZE_RIGHT),
		m_pDevice);
	
	// �������Ԃ̕\��
	m_pTimer = CCountTime::Create(D3DXVECTOR2(SCREEN_WIDTH*0.5f, 100), 99, m_pDevice);

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

	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pCrowdBar);
	SAFE_DELETE(m_pFace);
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
	
	// test
	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))
	{
		m_pStaminaBarL->UseStamina(50);
		m_pStaminaBarR->UseStamina(50);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
		m_pCrowdBar->Add(5);
	if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))
		m_pCrowdBar->Add(-5);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_UP))
	{
		if (m_pHpBarL)	m_pHpBarL->Add(20);
		if (m_pHpBarR)	m_pHpBarR->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
	{
		if (m_pHpBarL)	m_pHpBarL->Add(-20);
		if (m_pHpBarR)	m_pHpBarR->Add(-20);
	}
	
	// �A�j���[�V�������Ă�Ƃ�
	if (m_isAnimation){
		m_pHpBarR->Add(2);
		m_pHpBarL->Add(2);
		m_pCrowdBar->Replace(2);
	
		m_AnimationCount++;
		// �A�j���[�V�����I��
		if (m_AnimationCount > 120){
			m_isAnimation = false;
		}
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
void CUiManager::StartAnimation(void)
{
	// �o�[������
	m_pHpBarL->Reset();
	m_pHpBarR->Reset();
	m_pCrowdBar->Reset();

	m_isAnimation = true;
	m_AnimationCount = 0;
}

//----EOF----