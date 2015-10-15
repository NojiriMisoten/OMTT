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
const D3DXVECTOR2 FACE_POS_LEFT = D3DXVECTOR2(50, 50);
// ��E�T�C�h�̍��W
const D3DXVECTOR2 FACE_POS_RIGHT = D3DXVECTOR2(SCREEN_WIDTH - 50, 50);
// �獶�T�C�h�̑傫��
const D3DXVECTOR2 FACE_SIZE_LEFT = D3DXVECTOR2(50, 50);
// ��E�T�C�h�̑傫��
const D3DXVECTOR2 FACE_SIZE_RIGHT = D3DXVECTOR2(50, 50);

// �����Q�[�W�̍���
const float CROWD_HEIGHT = 30;

// HP�Q�[�W�̍����ƕ�
const float HP_WIDTH = 300;
const float HP_HEIGHT = 30;
// HP�Q�[�W�̒��S����̋���
const float HP_CENTER_DIST_X = 30;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_StaminaBarL = nullptr;
	m_StaminaBarR = nullptr;
	m_CrowdBar = nullptr;
	m_HpBarL = nullptr;
	m_HpBarR = nullptr;
	m_Face = nullptr;
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
	// �ϐ�������
	m_isAnimation = 0;
	m_AnimationCount = 0;

	// ���܃R���`�悵�ĂȂ�
	m_StaminaBarL = CStaminaBar::Create(
		D3DXVECTOR2(50, 100),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f - 50, 100),
		CStaminaBar::POSITIONBASE_LEFT, m_pDevice);
	m_StaminaBarR = CStaminaBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f + 50, 100),
		D3DXVECTOR2(SCREEN_WIDTH - 50, 100),
		CStaminaBar::POSITIONBASE_RIGHT, m_pDevice);

	// HP
	m_HpBarL = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_LEFT,
		m_pDevice);
	m_HpBarR = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_RIGHT,
		m_pDevice);

	// �ϐ�
	m_CrowdBar = CCrowdBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 50),
		CROWD_HEIGHT,
		m_pDevice);

	// ��
	m_Face = CFace::Create(
		FACE_POS_LEFT, FACE_SIZE_LEFT,
		FACE_POS_RIGHT, FACE_SIZE_RIGHT,
		m_pDevice);

	m_Timer = CCountTime::Create(D3DXVECTOR2(SCREEN_WIDTH*0.5f, 100), 99, m_pDevice);

}

//=============================================================================
// �I��
//=============================================================================
void CUiManager::Uninit(void)
{
	// ������̓C���X�^���X�������Ă邾��������delete���K�v
	SAFE_DELETE(m_Timer);
	SAFE_DELETE(m_CrowdBar);
	SAFE_DELETE(m_Face);
}

//=============================================================================
// �X�V
//=============================================================================
void CUiManager::Update(void)
{
	// �X�V
	m_CrowdBar->Update();
	m_Timer->Update();
	m_Face->Update();

	// test
	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))
	{
		m_StaminaBarL->UseStamina(50);
		m_StaminaBarR->UseStamina(50);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
		m_CrowdBar->Add(5);
	if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))
		m_CrowdBar->Add(-5);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_UP))
	{
		if (m_HpBarL)	m_HpBarL->Add(20);
		if (m_HpBarR)	m_HpBarR->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
	{
		if (m_HpBarL)	m_HpBarL->Add(-20);
		if (m_HpBarR)	m_HpBarR->Add(-20);
	}

	// �A�j���[�V�������Ă�Ƃ�
	if (m_isAnimation){
		m_HpBarR->Add(2);
		m_HpBarL->Add(2);
		m_CrowdBar->Replace(2);

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
	m_HpBarL->Reset();
	m_HpBarR->Reset();
	m_CrowdBar->Reset();

	m_isAnimation = true;
	m_AnimationCount = 0;
}

//----EOF----