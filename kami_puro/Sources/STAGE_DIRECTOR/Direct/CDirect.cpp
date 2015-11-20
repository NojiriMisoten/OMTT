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
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_1, CPlayer::PLAYER_WAIT );
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_2, CPlayer::PLAYER_WAIT );
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