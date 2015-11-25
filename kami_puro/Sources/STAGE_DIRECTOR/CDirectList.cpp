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
#include "Direct/CDirectSlap.h"
#include "Direct/CDirectBackdrop.h"


//=================================================
// コンストラクタ
//=================================================
CDirectList::CDirectList( CManager *pManager )
{
	m_pManager = pManager;

	for( int i = 0; i < DIR_MAX; i++ )
	{
		m_pDirectList[i] = NULL;
	}

	/*ここにいっぱい*/
	m_pDirectChop = NULL;
	m_pDirectElbow = NULL;
	m_pDirectLariat = NULL;
	m_pDirectSlap = NULL;
	m_pDirectBackdrop = NULL;
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
	m_pDirectList[0] = (CDirect *)new CDirectChop( m_pManager );
	m_pDirectList[1] = (CDirect *)new CDirectElbow( m_pManager );
	m_pDirectList[2] = (CDirect *)new CDirectLariat( m_pManager );
	m_pDirectList[3] = (CDirect *)new CDirectChop( m_pManager );
	m_pDirectList[4] = (CDirect *)new CDirectChop( m_pManager );
	m_pDirectList[5] = (CDirect *)new CDirectChop( m_pManager );
	m_pDirectList[6] = ( CDirect * )new CDirectSlap( m_pManager );
	m_pDirectList[7] = ( CDirect * )new CDirectBackdrop( m_pManager );
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