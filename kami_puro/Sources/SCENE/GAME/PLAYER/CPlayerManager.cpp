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
#include "CPlayer.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// static
//*****************************************************************************
CPlayer*	CPlayerManager::m_pPlayer[PLAYER_NUM] = {};
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
	m_pPlayer[1] = CPlayer::Create(pDevice, pos + D3DXVECTOR3(100.0f,0.0f,10.0f), type, m_pManager);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CPlayerManager::Uninit(void)
{
	//for (int i = 0; i < PLAYER_NUM; i++){
	//	if (m_pPlayer[i]){
	//		m_pPlayer[i]->Release();
	//		m_pPlayer[i] = NULL;
	//	}
	//}
}

//*****************************************************************************
// ゲッター
//*****************************************************************************
D3DXVECTOR3 CPlayerManager::GetPlayerPos(int num)
{
	return m_pPlayer[num]->GetPos();
}

int CPlayerManager::GetPlayerID(int num)
{
	return m_pPlayer[num]->GetID();
}

int CPlayerManager::GetPlayerHP(int num)
{
	return m_pPlayer[num]->GetHP();
}

int CPlayerManager::GetPlayerState(int num)
{
	return m_pPlayer[num]->GetAnimState();
}
//----EOF----