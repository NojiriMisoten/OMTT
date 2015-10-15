//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CPlayerManager.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// static
//*****************************************************************************
CPlayer*	CPlayerManager::m_pPlayer[PLAYER_NUM] = { NULL };
CManager*		CPlayerManager::m_pManager = NULL;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayerManager::CPlayerManager(CManager* pManager)
{
	m_pPlayer[0] = NULL;
	m_pPlayer[1] = NULL;
	m_pManager = pManager;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPlayerManager::~CPlayerManager(void)
{

}

//*****************************************************************************
// �쐬
//*****************************************************************************
void CPlayerManager::CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type)
{
	m_pPlayer[0] = CPlayer::Create(pDevice, pos, type, m_pManager);
	m_pPlayer[1] = CPlayer::Create(pDevice, pos + D3DXVECTOR3(100.0f, 0.0f, 10.0f), type, m_pManager);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CPlayerManager::Uninit(void)
{

}

//*****************************************************************************
// �Q�b�^�[
//*****************************************************************************
D3DXVECTOR3& CPlayerManager::GetPlayerPos(int ID)
{
	return m_pPlayer[ID]->GetPos();
}

int CPlayerManager::GetPlayerHP(int ID)
{
	return m_pPlayer[ID]->GetHP();
}

CPlayer::PLAYER_ANIM_TYPE CPlayerManager::GetPlayerState(int ID)
{
	return m_pPlayer[ID]->GetAnimState();
}
//----EOF----