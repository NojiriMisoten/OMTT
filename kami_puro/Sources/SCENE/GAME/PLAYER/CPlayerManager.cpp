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
CPlayer*	CPlayerManager::m_pPlayer = NULL;
CManager*		CPlayerManager::m_pManager = NULL;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayerManager::CPlayerManager(CManager* pManager)
{
	m_pPlayer = NULL;
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
	m_pPlayer = CPlayer::Create(pDevice, pos, type, m_pManager);
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
D3DXVECTOR3 CPlayerManager::GetPlayerPos(void)
{
	return m_pPlayer->GetPos();
}

//----EOF----