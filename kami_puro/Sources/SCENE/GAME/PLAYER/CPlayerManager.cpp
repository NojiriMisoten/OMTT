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

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayerManager::CPlayerManager(CManager* pManager)
{
	m_pPlayer[PLAYER_1] = NULL;
	m_pPlayer[PLAYER_2] = NULL;
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
	m_pPlayer[PLAYER_1] = CPlayer::Create(pDevice, (D3DXVECTOR3)DEFAULT_PLAYER_1_POS, (D3DXVECTOR3)DEFAULT_PLAYER_1_ROT, type, m_pManager, PLAYER_1);
	m_pPlayer[PLAYER_2] = CPlayer::Create(pDevice, (D3DXVECTOR3)DEFAULT_PLAYER_2_POS, (D3DXVECTOR3)DEFAULT_PLAYER_2_ROT,type, m_pManager, PLAYER_2);
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
D3DXVECTOR3& CPlayerManager::GetPlayerPos(PLAYER_ID ID)
{
	return m_pPlayer[ID]->GetPos();
}

int CPlayerManager::GetPlayerHP( PLAYER_ID ID )
{
	return m_pPlayer[ID]->GetHP();
}

CPlayer::PLAYER_ANIM_TYPE CPlayerManager::GetPlayerState( PLAYER_ID ID )
{
	return m_pPlayer[ID]->GetAnimState();
}

void CPlayerManager::SetAnimType( PLAYER_ID ID, int type , double moveRate)
{
	m_pPlayer[ID]->SetAnimType( type , moveRate);
}

void CPlayerManager::SetAnimMortionOfTime(PLAYER_ID ID, double time )
{
	m_pPlayer[ID]->SetAnimMortionOfTime(time );
}

void CPlayerManager::SetAnimSpd(PLAYER_ID ID, double spd)
{
	m_pPlayer[ID]->SetAnimSpd(spd);
}

bool CPlayerManager::GetUseFinishFlag(PLAYER_ID ID)
{
	return m_pPlayer[ID]->GetUseFinishFlag();
}

void CPlayerManager::SetUseFinishFlag(PLAYER_ID ID, bool flag)
{
	m_pPlayer[ID]->SetUseFinishFlag(flag);
}

bool CPlayerManager::GetRopeFlag(PLAYER_ID ID)
{
	return m_pPlayer[ID]->GetRopeFlag();
}

void CPlayerManager::SetRopeFlag(PLAYER_ID ID, bool flag)
{
	m_pPlayer[ID]->SetRopeFlag(flag);
}

void CPlayerManager::ReverseRopeFlag(PLAYER_ID ID)
{
	m_pPlayer[ID]->ReverseRopeFlag();
}

D3DXVECTOR3& CPlayerManager::GetPlayerRot(PLAYER_ID ID)
{
	return m_pPlayer[ID]->GetRot();
}

//----EOF----