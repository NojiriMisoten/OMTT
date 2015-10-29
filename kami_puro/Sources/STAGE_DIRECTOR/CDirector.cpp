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

//=================================================
// コンストラクタ
//=================================================
CDirector::CDirector( void )
{
	m_IsDirecting = false;
	m_CurAttackID = (ATTACK_LIST)-1;
	m_CurPlayerID = -1;
}

//=================================================
// デストラクタ
//=================================================
CDirector::~CDirector( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirector::Init( void )
{

}

//=================================================
// 終了
//=================================================
void CDirector::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirector::Update( void )
{
	// 攻撃演出無しの場合
	if( !m_IsDirecting )
	{
		// カメラとかの設定

	}

	// 攻撃演出有りの場合
	else
	{
		// 再生するAttackID
		switch( m_CurAttackID )
		{
		case ATTACK_SMALL_CHOP:
			break;

		case ATTACK_SMALL_ELBOW:
			break;

		case ATTACK_SMALL_LARIAT:
			break;

		case ATTACK_BIG_ROLLING:
			break;

		case ATTACK_BIG_SHOLDER:
			break;

		case ATTACK_BIG_DROPKICK:
			break;

		case ATTACK_THROW_SLAP:
			break;

		case ATTACK_THROW_BACKDROP:
			break;

		case ATTACK_THROW_STUNNER:
			break;

		case ATTACK_FINISH_BODYPRESS:
			break;

		case ATTACK_FINSH_ATOMICBUSTER:
			break;
		}
	}
}

//=================================================
// ディレクターに再生する演出を送る
//=================================================
void CDirector::SendDirector( ATTACK_LIST attackID, int playerID )
{
	m_IsDirecting = true;
	m_CurAttackID = attackID;
	m_CurPlayerID = playerID;
}

//----EOF----