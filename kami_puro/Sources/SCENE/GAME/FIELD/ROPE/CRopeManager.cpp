//=============================================================================
//
// CRopeManagerManager�N���X [CRopeManagerManager.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CRopeManager.h"
#include "CRope.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �x���W
static const float ROPE_Y = 45;
// ��
static const float ROPE_WIDTH = 130;
// ����
static const float ROPE_HEIGHT = 40;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRopeManager::CRopeManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pD3DDevice = pDevice;
	m_pManager = pManager;

	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRope[i] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRopeManager::~CRopeManager(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CRopeManager::Init()
{
	// ��O
	m_pRope[0] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, -40), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[0]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

	// ��
	m_pRope[1] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, 40), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[1]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI, 0));

	// ��
	m_pRope[2] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(-40, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[2]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));

	// �E
	m_pRope[3] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(40, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[3]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
}

//=============================================================================
// �I��
//=============================================================================
void CRopeManager::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CRopeManager::Update(void)
{
}

//=============================================================================
// �S���A�j���\�V�����X�^�[�g
// �ǂ̃��[�v���A��������́A���̎�𗣂��܂ł̃t���[���J�E���g
//=============================================================================
void CRopeManager::Pull(RopeNum num, float pullPower, int pullInterval)
{
#ifdef _DEBUG
	assert((num >= 0) && (num < RopeNumMax) && "���[�v�̔ԍ�������ĂˁI");
#endif
	m_pRope[num]->Pull(pullPower, pullInterval);
}

//=============================================================================
// �`�悵�Ȃ�
//=============================================================================
void CRopeManager::SetImvisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRope[i]->SetDrawFlag(false);
	}
}

//=============================================================================
// �`�悷��
//=============================================================================
void CRopeManager::SetVisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRope[i]->SetDrawFlag(true);
	}
}

//----EOF----