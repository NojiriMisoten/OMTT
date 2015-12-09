//=============================================================================
//
// CFieldManager�N���X [CFieldManager.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MANAGER/CManager.h"
#include "CFieldManager.h"
#include "ROPE\CRopeManager.h"
#include "../../../BASE_OBJECT/CSceneX.h"

//*****************************************************************************
// �萔
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CFieldManager::CFieldManager(
	LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pDevice = pDevice;
	m_pManager = pManager;

	m_pRopeManger = NULL;
	m_pRing = NULL;
	m_pFrontChair = NULL;
	m_pOverChair = NULL;
	m_pLeftChair = NULL;
	m_pStage = NULL;
	m_pRightChair = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CFieldManager::~CFieldManager(void)
{

}

//*****************************************************************************
// ����������
//*****************************************************************************
void CFieldManager::Init(void)
{
	// ���[�v�̐���
	m_pRopeManger = new CRopeManager(m_pDevice, m_pManager);
	m_pRopeManger->Init();

	// �����O
	m_pRing = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_RING, m_pManager);
	m_pRing->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pRing->SetRot(D3DXVECTOR3(0, D3DX_PI, 0));

	// �֎q
	m_pFrontChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_FRONT_CHAIR, m_pManager);
	m_pFrontChair->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pOverChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_OVER_CHAIR, m_pManager);
	m_pOverChair->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pLeftChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_SIDE_CHAIR, m_pManager);
	m_pLeftChair->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pLeftChair->SetRot(D3DXVECTOR3(0, D3DX_PI, 0));
	m_pRightChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_SIDE_CHAIR, m_pManager);
	m_pRightChair->SetScl(D3DXVECTOR3(50, 50, 50));

	// �X�e�[�W
	m_pStage = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_STAGE, m_pManager);
	m_pStage->SetScl(D3DXVECTOR3(50, 50, 50));
}

//*****************************************************************************
// �쐬
//*****************************************************************************
CFieldManager *CFieldManager::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	CManager *pManager)
{
	CFieldManager* p = new CFieldManager(pDevice, pManager);
	p->Init();
	return p;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CFieldManager::Uninit(void)
{
	m_pRopeManger->Uninit();
	SAFE_DELETE(m_pRopeManger);
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CFieldManager::Update(void)
{
	// test
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_ROPE_BIYON))
	{
		m_pRopeManger->Pull(CRopeManager::RopeNumLeft, 5.6f, 90);
	}
}

//*****************************************************************************
// �`�悵�Ȃ�
//*****************************************************************************
void CFieldManager::SetImvisible(void)
{
	m_pRopeManger->SetImvisible();
	m_pRing->SetDrawFlag(false);
	m_pFrontChair->SetDrawFlag(false);
	m_pOverChair->SetDrawFlag(false);
	m_pLeftChair->SetDrawFlag(false);
	m_pStage->SetDrawFlag(false);
	m_pRightChair->SetDrawFlag(false);
}

//*****************************************************************************
// �`�悷��
//*****************************************************************************
void CFieldManager::SetVisible(void)
{
	m_pRopeManger->SetVisible();
	m_pRing->SetDrawFlag(true);
	m_pFrontChair->SetDrawFlag(true);
	m_pOverChair->SetDrawFlag(true);
	m_pLeftChair->SetDrawFlag(true);
	m_pStage->SetDrawFlag(true);
	m_pRightChair->SetDrawFlag(true);
}
//----EOF----