//=============================================================================
//
// CDirectクラス [CDirect.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirect.h"
#include "../../MANAGER/CManager.h"
#include "../CDirectorManager.h"
#include "../../SCENE/GAME/JUDGE/CJudgeManager.h"

//=================================================
// コンストラクタ
//=================================================
CDirect::CDirect( CManager *pManager )
{
	m_pManager = pManager;
	m_FrameCount = 0;
}

//=================================================
// デストラクタ
//=================================================
CDirect::~CDirect( void )
{

}


//=================================================
// 演出が終了したかチェック
//=================================================
void CDirect::CheckEnd( void )
{
	m_FrameCount++;
	if( m_FrameCount > m_TotalFrame )
	{
		m_pManager->GetDirectorManager()->SetEndDirecting();

		// モーションリセット
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_1, CPlayer::PLAYER_WAIT );
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_2, CPlayer::PLAYER_WAIT );

		// 座標リセット
		m_pManager->GetPlayerManager()->SetPos( PLAYER_1, m_pManager->GetJudgeManager()->GetSavePos( PLAYER_1 ) );
		m_pManager->GetPlayerManager()->SetPos( PLAYER_2, m_pManager->GetJudgeManager()->GetSavePos( PLAYER_2 ) );

		// 回転リセット
		m_pManager->GetPlayerManager()->SetRot( PLAYER_1, (D3DXVECTOR3)DEFAULT_PLAYER_1_ROT );
		m_pManager->GetPlayerManager()->SetRot( PLAYER_2, (D3DXVECTOR3)DEFAULT_PLAYER_2_ROT );

		//　アニメーション速度リセット
		m_pManager->GetPlayerManager()->SetAnimSpd( PLAYER_1, DEFFAULT_ANIM_SPD );
		m_pManager->GetPlayerManager()->SetAnimSpd( PLAYER_2, DEFFAULT_ANIM_SPD );
	}
}

void CDirect::SetPlayerID( PLAYER_ID playerID )
{
	// プレイヤーIDを割り当て
	m_Player = playerID;
	if( playerID == PLAYER_1 )
	{
		m_Enemy = PLAYER_2;
	}
	else
	{
		m_Enemy = PLAYER_1;
	}
}

//----EOF----