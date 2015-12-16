//=============================================================================
//
// CDirectorクラス [CDirector.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirector.h"
#include "../MANAGER/CManager.h"
#include "../CAMERA/CameraManager.h"
#include "../EFECT/CEffectManager.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "CDirectList.h"
#include "Direct\CDirect.h"

//=================================================
// コンストラクタ
//=================================================
CDirector::CDirector( CManager *pManager, CGame *pGame )
{
	m_pManager = pManager;
	m_pGame = pGame;
	m_CueDirect = DIRECT_PLAYER{ (DIRECT_ID)-1, (PLAYER_ID)-1 };
	m_CurDirect = DIRECT_PLAYER{ (DIRECT_ID)-1, (PLAYER_ID)-1 };
}

//=================================================
// デストラクタ
//=================================================
CDirector::~CDirector( void )
{
	m_pManager = NULL;
}

//=================================================
// 初期化
//=================================================
void CDirector::Init( void )
{
	// 演出リストを作成
	m_pDirectList = new CDirectList( m_pManager, m_pGame );
	m_pDirectList->Init();
}

//=================================================
// 終了
//=================================================
void CDirector::Uninit( void )
{
	m_pDirectList->Uninit();
	SAFE_DELETE( m_pDirectList );
}

//=================================================
// 更新
//=================================================
void CDirector::Update( void )
{
	// 攻撃演出無しの場合
	if( m_CurDirect.directingID < 0 )
	{
		// 予約が入っていれば
		if( m_CueDirect.directingID != (DIRECT_ID)-1 )
		{
			// 代入
			m_CurDirect = m_CueDirect;
			m_CueDirect = DIRECT_PLAYER{ (DIRECT_ID)-1, (PLAYER_ID)-1 };

			// 演出初期化
			m_pDirectList->GetDirectList( m_CurDirect.directingID )->Init( m_CurDirect.playerID );
		}
	}

	// 攻撃演出有りの場合
	else
	{
		// 演出を更新
		m_pDirectList->GetDirectList( m_CurDirect.directingID )->Update();
	}
}


//=================================================
// ディレクターに再生する演出を送る
//=================================================
void CDirector::SendDirector( DIRECT_ID directingID, PLAYER_ID playerID )
{
	m_CurDirect = DIRECT_PLAYER{ directingID, playerID };
}

int CDirector::Direct( DIRECT_ID cueDirectingID, PLAYER_ID cuePlayerID )
{
	// 再生中じゃなければ予約
	if( GetIsDirecting().directingID < (DIRECT_ID)0 )
	{
		m_CueDirect = DIRECT_PLAYER{ cueDirectingID, cuePlayerID };
		return 1;
	}

	return 0;
}

void CDirector::SetEndDirecting( void )
{
	m_CurDirect.directingID = (DIRECT_ID)-1;
	SetDefaultCamera();
}

void CDirector::SetDefaultCamera( void )
{
	D3DXVECTOR3 playerCenter = (
		m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 )
		+ m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 ) )
		/ 2;

	m_pManager->GetCameraManager()->CameraSetToCoord(
		D3DXVECTOR3( playerCenter.x, 90.0f, -100.0f ),
		D3DXVECTOR3( playerCenter.x, 70.0f, 0.0f ) );
}

//----EOF----