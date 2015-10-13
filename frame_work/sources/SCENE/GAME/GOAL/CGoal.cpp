//=============================================================================
//
// CGoalManagerクラス [CGoalManager.cpp]
// Author : 塚本　俊彦
//
// 宝物をここに運ぶ α版は絵だけ
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CGoal.h"
#include "../CGame.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float GOAL_WIDTH = 60;
static const float GOAL_HEIGHT = 60;

// 転送する時間（1段階目<first>の次は2段階目<second>）
static const int TRANSE_INTERVAL_FIRST = 60;
static const int TRANSE_INTERVAL_SECOND = 120;
static const int TRANSE_INTERVAL_THIRD = 180;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGoal::CGoal(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) : CScene2D(pDevice, nPriority, m_objType)
{
	m_State = GOAL_STATE_NONE;
	m_nCount = 0;
	m_bTransFlg = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGoal ::~CGoal(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CGoal::Init()
{

}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGoal::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGoal::Update(void)
{
	// 到着したとき
	if (m_State == GOAL_STATE_ARRIVE){
		// 初期処理
		m_nCount = 0;

		// 転送一段階目にする
		m_State = GOAL_STATE_TRANS_FIRST;
	}
	else if (m_State == GOAL_STATE_TRANS_FIRST){
		m_nCount++;
		if (m_nCount == TRANSE_INTERVAL_FIRST){
			m_State = GOAL_STATE_TRANS_SECOND;
		}
	}
	else if (m_State == GOAL_STATE_TRANS_SECOND){
		m_nCount++;
		if (m_nCount == TRANSE_INTERVAL_SECOND){
			m_State = GOAL_STATE_TRANS_THIRD;
		}
	}
	else if (m_State == GOAL_STATE_TRANS_THIRD){
		m_nCount++;
		if (m_nCount == TRANSE_INTERVAL_THIRD){
			m_State = GOAL_STATE_TRANSED;
		}
	}
	else if (m_State == GOAL_STATE_TRANSED){
		// ゲーム終了
		m_pGame->SetGameOver();
	}

#ifdef _DEBUG
	if (m_State == GOAL_STATE_ARRIVE){
		CDebugProc::Print("GOAL_STATE_ARRIVE\n");
	}
	else if (m_State == GOAL_STATE_TRANS_FIRST){
		CDebugProc::Print("GOAL_STATE_FIRST\n");
	}
	else if (m_State == GOAL_STATE_TRANS_SECOND){
		CDebugProc::Print("GOAL_STATE_SECOND\n");
	}
	else if (m_State == GOAL_STATE_TRANS_THIRD){
		CDebugProc::Print("GOAL_STATE_THIRD\n");
	}
	else if (m_State == GOAL_STATE_TRANSED){
		CDebugProc::Print("GOAL_STATE_TRANSED\n");
	}
#endif
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CGoal* CGoal::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, short nPlayerNum, CGame *pGame)
{
	// 作成
	CGoal* p = new CGoal(pDevice);

	// プレイヤーの色に合わせたゴールの丸のテクスチャ
	TEXTURE_TYPE tex = TEXTURE_NULL;
	if (nPlayerNum == 0){
		tex = TEXTURE_SPAWN_YELLOW;
	}
	else if (nPlayerNum == 1){
		tex = TEXTURE_SPAWN_RED;
	}
	else if (nPlayerNum == 2){
		tex = TEXTURE_SPAWN_BLUE;
	}
	else if (nPlayerNum == 3){
		tex = TEXTURE_SPAWN_GREEN;
	}
	p->CScene2D::Init(pos, GOAL_WIDTH, GOAL_HEIGHT, tex);
	//	p->SetDrawFlag(false);

	p->m_nPlayerNum = nPlayerNum;
	p->m_pGame = pGame;
	return p;
}

//*****************************************************************************
// 転送中にセットする　Judgeの更新で宝を持ったプレイヤがゴールにいれば常に呼ばれる
//*****************************************************************************
void CGoal::SetTrans(){

	// すでにセットされているならなにもしない
	if (m_bTransFlg){
		return;
	}
	else{
		// ゴールに着いた
		m_State = GOAL_STATE_ARRIVE;
		m_pGame->SetWinPlayer(m_nPlayerNum);
		m_bTransFlg = true;
	}
}

//*****************************************************************************
// 転送中じゃないセットする　
//*****************************************************************************
void CGoal::SetNoTrans(){

	m_State = GOAL_STATE_NONE;
	m_bTransFlg = false;
}

//----EOF-------