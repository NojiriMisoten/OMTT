//=============================================================================
//
// CJudgeManagerクラス [CJudgeManager.cpp]
// Author : 池島　大樹
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CJudgeManager.h"
#include "CJudge.h"
#include "../../../CONST/const.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CJudgeManager::CJudgeManager( void )
{
	m_pJudge = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CJudgeManager::~CJudgeManager( void )
{

}

//=============================================================================
// 初期化
//=============================================================================
void CJudgeManager::Init( CManager* pManager )
{
	m_pJudge = CJudge::Create( pManager );
}

//=============================================================================
// 終了
//=============================================================================
void CJudgeManager::Uninit( void )
{
	if( m_pJudge )
	{
		m_pJudge->Uninit();
		delete m_pJudge;
		m_pJudge = NULL;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CJudgeManager::Update( void )
{
	m_pJudge->Update();
}

//----EOF----