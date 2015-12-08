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
static const float ROPE_Y = 37;
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
		m_pRopeOut[i] = NULL;
		m_pRopeIn[i] = NULL;
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
	m_pRopeOut[0] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, -10), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[0]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
	m_pRopeIn[0] = CRope::Create(m_pD3DDevice,
		// ���� -5 �ɂ��Ȃ��ƃ��[�v�������A�����s��
		D3DXVECTOR3(0, ROPE_Y - 5, -157), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[0]->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0, 0));


	// ��
	m_pRopeOut[1] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, 10), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[1]->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0, 0));
	m_pRopeIn[1] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, 157), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[1]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));


	// ��
	m_pRopeOut[2] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(-10, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[2]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));
	m_pRopeIn[2] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(-157, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[2]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));


	// �E
	m_pRopeOut[3] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(10, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[3]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
	m_pRopeIn[3] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(157, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[3]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));

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
	assert((num >= 0) && (num < RopeNumMax) && "���[�v�̔ԍ�������ĂˁI");

	m_pRopeOut[num]->Pull(pullPower, pullInterval);
	m_pRopeIn[num]->Pull(-pullPower, pullInterval);
}

//=============================================================================
// �`�悵�Ȃ�
//=============================================================================
void CRopeManager::SetImvisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRopeOut[i]->SetDrawFlag(false);
		m_pRopeIn[i]->SetDrawFlag(false);
	}
}

//=============================================================================
// �`�悷��
//=============================================================================
void CRopeManager::SetVisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRopeOut[i]->SetDrawFlag(true);
		m_pRopeIn[i]->SetDrawFlag(true);
	}
}

//----EOF----