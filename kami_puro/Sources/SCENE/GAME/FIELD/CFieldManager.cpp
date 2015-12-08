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
	m_pDome = NULL;
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

	// �ϋq��
	m_pDome = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_DOME, m_pManager);
	m_pDome->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pDome->SetRot(D3DXVECTOR3(0, D3DX_PI*0.5f, 0));
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
	m_pDome->SetDrawFlag(false);
}

//*****************************************************************************
// �`�悷��
//*****************************************************************************
void CFieldManager::SetVisible(void)
{
	m_pRopeManger->SetVisible();
	m_pDome->SetDrawFlag(true);
}
//----EOF----