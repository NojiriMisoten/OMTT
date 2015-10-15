//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CPlayerManager.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// static
//*****************************************************************************
CPlayer*	CPlayerManager::m_pPlayer[PLAYER_NUM] = { NULL };
CManager*		CPlayerManager::m_pManager = NULL;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayerManager::CPlayerManager(CManager* pManager)
{
	m_pPlayer[0] = NULL;
	m_pPlayer[1] = NULL;
	m_pManager = pManager;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPlayerManager::~CPlayerManager(void)
{

}

//*****************************************************************************
// 作成
//*****************************************************************************
void CPlayerManager::CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type)
{
	m_pPlayer[0] = CPlayer::Create(pDevice, pos, type, m_pManager);
	m_pPlayer[1] = CPlayer::Create(pDevice, pos + D3DXVECTOR3(100.0f, 0.0f, 10.0f), type, m_pManager);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CPlayerManager::Uninit(void)
{

}

//*****************************************************************************
// ゲッター
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