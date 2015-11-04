//=============================================================================
//
// CLightManager�N���X [CLightManager.cpp]
// Author : ����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CLightManager.h"
#include "../MANAGER/CManager.h"

//===========================================================================
// �萔
//===========================================================================
// ���C�g����]�����Ƃ��̓����͈�
static const float LIGHT_DIST = 100.0f;
// ���C�g��1�t���[���̉�]���x
static const float LIGHT_ROT = 0.01f;
// ���C�g�̂x���W
static const float LIGHT_Y = 10.f;

//===========================================================================
// �R���X�g���N�^
//===========================================================================
CLightManager::CLightManager(CManager* pManager)
{
	m_pManager = pManager;
	m_Count = 0;

	m_RadOne = D3DX_PI * 2.0f / LIGHT_NUM;

	// ���C�g�̐���
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		m_pLight[i] = new CLight();
	}

	// �����ʒu
	float rad = m_Count;
	for (int i = 0; i < LIGHT_NUM; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST);
		float pos_z = sinf(rad) * (LIGHT_DIST);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y, pos_z));
		rad += m_RadOne;
	}

	// test�@�q�f�a�Ŋm�F�ł���
	//	m_pLight[0]->SetColor(D3DXCOLOR(1, 0, 0, 1));
	//	m_pLight[1]->SetColor(D3DXCOLOR(0, 1, 0, 1));
	//	m_pLight[2]->SetColor(D3DXCOLOR(0, 0, 1, 1));
}

//===========================================================================
// �f�X�g���N�^
//===========================================================================
CLightManager::~CLightManager(void)
{

}

//===========================================================================
// �I��
//===========================================================================
void CLightManager::Uninit(void)
{
	// ���C�g�̍폜
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		SAFE_DELETE(m_pLight[i]);
	}
}

//===========================================================================
// �X�V
//===========================================================================
void CLightManager::Update(void)
{
	m_Count += LIGHT_ROT;
	NormalizeRotation(&m_Count);

	// �ړ�
	float rad = m_Count;
	for (int i = 0; i < LIGHT_NUM; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST);
		float pos_z = sinf(rad) * (LIGHT_DIST);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y, pos_z));
		rad += m_RadOne;
	}
}

//----EOF----