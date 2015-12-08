//=============================================================================
//
// CCrowdManager�N���X [CCrowdManager.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCrowdManager.h"
#include "CCrowd.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �����O�̑O��̊ϋq�̔|���̂�������
static const float CROWD_WIDTH = 300;
static const float CROWD_HEIGHT = 70;
// �����O�O���̊ϋq�̈ʒu
static const D3DXVECTOR3 CROWD_POS_FRONT = D3DXVECTOR3(0, 30, 120);
// �����O����̊ϋq�̈ʒu
static const D3DXVECTOR3 CROWD_POS_BACK = D3DXVECTOR3(0, 30, -120);

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCrowdManager::CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pD3DDevice = pDevice;
	m_pManager = pManager;
	m_pCrowd3DFront = NULL;
	m_pCrowd3DBack = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCrowdManager ::~CCrowdManager(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CCrowdManager::Init()
{
	// �����O�̑O���̊ϋq�̔|������
	m_pCrowd3DFront = CCrowd::Create(m_pD3DDevice,
		D3DXVECTOR3(CROWD_POS_FRONT),
		CROWD_WIDTH, CROWD_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
	m_pCrowd3DFront->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

	// �����O�̌���̊ϋq�̔|������
	m_pCrowd3DBack = CCrowd::Create(m_pD3DDevice,
		D3DXVECTOR3(CROWD_POS_BACK),
		CROWD_WIDTH, CROWD_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
	m_pCrowd3DBack->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CCrowdManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CCrowdManager::Update(void)
{
}


//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CCrowdManager* CCrowdManager::Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	// �쐬
	CCrowdManager* p = new CCrowdManager(pDevice, pManager);

	// ������
	p->Init();

	return p;

}

//*****************************************************************************
// �`�悵�Ȃ�
//*****************************************************************************
void CCrowdManager::SetImvisible(void)
{
	m_pCrowd3DFront->SetDrawFlag(false);
	m_pCrowd3DBack->SetDrawFlag(false);
}

//*****************************************************************************
// �`�悷��
//*****************************************************************************
void CCrowdManager::SetVisible(void)
{
	m_pCrowd3DFront->SetDrawFlag(true);
	m_pCrowd3DBack->SetDrawFlag(true);
}
//----EOF----