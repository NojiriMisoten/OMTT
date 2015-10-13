//=============================================================================
//
// CBackGroundManager�N���X [CBackGroundManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CBackGroundManager.h"
#include "../SCENE/CSCENE/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const D3DXVECTOR3 BG_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
static const D3DXVECTOR3 BG_POS_TWICE_L = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT * 0.5f, 0);
static const D3DXVECTOR3 BG_POS_TWICE_R = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT * 0.5f, 0);
static const UV_INDEX BG_UV_TWICE = { 0.0f, 2.0f, 0.0f, 1.0f };
static const float BG_WIDTH = SCREEN_WIDTH;
static const float BG_HEIGHT = SCREEN_HEIGHT;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CBackGroundManager::CBackGroundManager(LPDIRECT3DDEVICE9 *pDevice)
{
	// ������
	m_pDevice = pDevice;
	m_nBackGroundNum = 0;

	for (int i = 0; i < BACKGROUND_MAX; i++){
		m_vPosition[i] = D3DXVECTOR3(0, 0, 0);
		m_bAnimation[i] = false;
		m_pSprite[i] = NULL;
		m_bDirection[i] = BG_DIRECTION_NONE;
	}
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CBackGroundManager ::~CBackGroundManager(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CBackGroundManager::Init()
{
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CBackGroundManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CBackGroundManager::Update(void){

	// ���������w�i�̍X�V
	for (int i = 0; i < BACKGROUND_MAX; i++){
		// ���̂��Ȃ����A�j���[�V�������Ȃ����͍̂X�V���Ȃ�
		if (!(m_bAnimation[i]) || !(m_pSprite[i]))	continue;

		// �ړ�
		m_vPosition[i].x += m_fVelocityX[i];
		m_pSprite[i]->SetPos(m_vPosition[i]);

		// �������ƂɃ��[�v����
		if (m_bDirection[i] == BG_DIRECTION_LEFT){
			if (m_vPosition[i].x <= 0){
				m_vPosition[i] = BG_POS_TWICE_L;
			}
		}
		else{
			if (m_vPosition[i].x >= SCREEN_WIDTH){
				m_vPosition[i] = BG_POS_TWICE_R;
			}
		}
	}
}

//*****************************************************************************
// �w�i�̐���
//*****************************************************************************
bool CBackGroundManager::CreateBG(TEXTURE_TYPE texture){

	// �ő�܂ō���Ă����玸�s
	if (m_nBackGroundNum == BACKGROUND_MAX) return false;

	m_pSprite[m_nBackGroundNum] = new CScene2D(m_pDevice, TYPE_PRIORITY_BG);
	m_pSprite[m_nBackGroundNum]->Init(BG_POS, BG_WIDTH, BG_HEIGHT, texture, 1, 1);
	m_vPosition[m_nBackGroundNum] = BG_POS;

	m_nBackGroundNum++;

	return true;
}

//*****************************************************************************
// �w�i�̐��� UV�A�j���[�V���������
//*****************************************************************************
bool CBackGroundManager::CreateBG(TEXTURE_TYPE texture, float velocity){

	// �ő�܂ō���Ă����玸�s
	if (m_nBackGroundNum == BACKGROUND_MAX) return false;

	m_pSprite[m_nBackGroundNum] = new CScene2D(m_pDevice, TYPE_PRIORITY_BG);
	if (velocity < 0){
		m_bDirection[m_nBackGroundNum] = BG_DIRECTION_LEFT;
		m_pSprite[m_nBackGroundNum]->Init(BG_POS_TWICE_L, BG_WIDTH * 2, BG_HEIGHT, texture, 1, 1);
		m_vPosition[m_nBackGroundNum] = BG_POS_TWICE_L;
	}
	else{
		m_bDirection[m_nBackGroundNum] = BG_DIRECTION_RIGHT;
		m_pSprite[m_nBackGroundNum]->Init(BG_POS_TWICE_R, BG_WIDTH * 2, BG_HEIGHT, texture, 1, 1);
		m_vPosition[m_nBackGroundNum] = BG_POS_TWICE_R;
	}
	m_pSprite[m_nBackGroundNum]->SetUV(const_cast<UV_INDEX*>(&BG_UV_TWICE));
	m_fVelocityX[m_nBackGroundNum] = velocity;
	m_bAnimation[m_nBackGroundNum] = true;

	m_nBackGroundNum++;
	return true;
}

//----EOF-------