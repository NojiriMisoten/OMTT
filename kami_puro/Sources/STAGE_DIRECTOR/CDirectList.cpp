//=============================================================================
//
// CDirectListクラス [CDirectList.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectList.h"
#include "../MANAGER/CManager.h"
#include "CDirectorManager.h"

/*いっぱいここに*/
#include "Direct/CDirectChop.h"
#include "Direct/CDirectElbow.h"
#include "Direct/CDirectLariat.h"

#include "Direct/CDirectRolling.h"
#include "Direct/CDirectShoulder.h"
#include "Direct/CDirectDropkick.h"

#include "Direct/CDirectSlap.h"
#include "Direct/CDirectBackdrop.h"
#include "Direct/CDirectStunner.h"

#include "Direct/CDirectFinisher.h"

#include "Direct/CDirectRope.h"

#include "Direct/CDirectLariatRope.h"
#include "Direct/CDirectDropkickRope.h"
#include "Direct/CDirectStunnerRope.h"

#include "Direct/CDirectBattleIntro.h"
#include "Direct/CDirectBattleResult.h"

//=================================================
// コンストラクタ
//=================================================
CDirectList::CDirectList( CManager *pManager, CGame *pGame )
{
	m_pManager = pManager;
	m_pGame = pGame;

	for( int i = 0; i < DIR_MAX; i++ )
	{
		m_pDirectList[i] = NULL;
	}

	/*ここにいっぱい*/
	m_pDirectChop = NULL;
	m_pDirectElbow = NULL;
	m_pDirectLariat = NULL;
	m_pDirectRolling = NULL;
	m_pDirectShoulder = NULL;
	m_pDirectDropkick = NULL;
	m_pDirectSlap = NULL;
	m_pDirectBackdrop = NULL;
	m_pDirectStunner = NULL;
	m_pDirectFinisher = NULL;
	m_pDirectRope = NULL;
	m_pDirectLariatRope = NULL;
	m_pDirectDropkickRope = NULL;
	m_pDirectStunnerRope = NULL;
	m_pDirectBattleIntro = NULL;
	m_pDirectBattleResult = NULL;
}

//=================================================
// デストラクタ
//=================================================
CDirectList::~CDirectList( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectList::Init( void )
{
	// リストにポインタを格納
	/*ここにいっぱい*/
	m_pDirectList[0] = ( CDirect * )new CDirectChop( m_pManager, m_pGame );
	m_pDirectList[1] = ( CDirect * )new CDirectElbow( m_pManager, m_pGame );
	m_pDirectList[2] = ( CDirect * )new CDirectLariat( m_pManager, m_pGame );
	m_pDirectList[3] = ( CDirect * )new CDirectRolling( m_pManager, m_pGame );
	m_pDirectList[4] = ( CDirect * )new CDirectShoulder( m_pManager, m_pGame );
	m_pDirectList[5] = ( CDirect * )new CDirectDropkick( m_pManager, m_pGame );
	m_pDirectList[6] = ( CDirect * )new CDirectSlap( m_pManager, m_pGame );
	m_pDirectList[7] = ( CDirect * )new CDirectBackdrop( m_pManager, m_pGame );
	m_pDirectList[8] = ( CDirect * )new CDirectStunner( m_pManager, m_pGame );
	m_pDirectList[9] = ( CDirect * )new CDirectFinisher( m_pManager, m_pGame );
	m_pDirectList[10] = ( CDirect * )new CDirectRope( m_pManager, m_pGame );
	m_pDirectList[11] = ( CDirect * )new CDirectLariatRope( m_pManager, m_pGame );
	m_pDirectList[12] = ( CDirect * )new CDirectDropkickRope( m_pManager, m_pGame );
	m_pDirectList[13] = ( CDirect * )new CDirectStunnerRope( m_pManager, m_pGame );
	m_pDirectList[14] = ( CDirect * )new CDirectBattleIntro( m_pManager, m_pGame );
	m_pDirectList[15] = ( CDirect * )new CDirectBattleResult( m_pManager, m_pGame );
}

//=================================================
// 終了
//=================================================
void CDirectList::Uninit( void )
{
	for( int i = 0; i < DIR_MAX; i++ )
	{
		if( m_pDirectList[i] != NULL )
		{
			SAFE_DELETE( m_pDirectList[i] );
		}
	}
}

//=================================================
// 更新
//=================================================
void CDirectList::Update( void )
{

}


//----EOF----