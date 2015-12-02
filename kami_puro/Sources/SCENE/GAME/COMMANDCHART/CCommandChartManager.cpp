//=============================================================================
//
//	CCommandChartManagerクラスの処理(CCommandChartManager.cpp)
//
//	Author : 佐藤　諒一
//
//=============================================================================
// インクルード
#include "CCommandChartManager.h"
#include "../../../CONST/const.h"

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CCommandChartManager::CCommandChartManager(CPlayerManager* pPlayerManager)
{
	// コマンドチャートクラス格納用のポインタをNULLに
	m_pCommandChart[PLAYER_1] = NULL;
	m_pCommandChart[PLAYER_2] = NULL;

	m_pPlayerManager = pPlayerManager;
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
CCommandChartManager::~CCommandChartManager()
{
}

//-----------------------------------------------------------------------------
//	生成処理
//-----------------------------------------------------------------------------
CCommandChartManager* CCommandChartManager::Create(LPDIRECT3DDEVICE9* pDevice, CPlayerManager* pPlayerManager)
{
	CCommandChartManager* temp = new CCommandChartManager(pPlayerManager);
	// コマンドチャートマネージャの初期化
	temp->Init(pDevice);

	return temp;
}

//-----------------------------------------------------------------------------
//	初期化処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Init(LPDIRECT3DDEVICE9* pDevice)
{
	// コマンドチャートの生成
	m_pCommandChart[PLAYER_1] = CCommandChart::Create(pDevice, PLAYER_1, this);
	m_pCommandChart[PLAYER_2] = CCommandChart::Create(pDevice, PLAYER_2, this);
}

//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Update(void)
{
	// コマンドチャートの更新
	m_pCommandChart[PLAYER_1]->Update();
	m_pCommandChart[PLAYER_2]->Update();

	Draw();
}

//-----------------------------------------------------------------------------
//	描画処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Draw(void)
{
	// コマンドチャートの描画
	m_pCommandChart[PLAYER_1]->Draw();
	m_pCommandChart[PLAYER_2]->Draw();
}

//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Uninit(void)
{
	// コマンドチャートの終了
	m_pCommandChart[PLAYER_1]->Uninit();
	m_pCommandChart[PLAYER_2]->Uninit();
	
	// 後片付け
	SAFE_DELETE(m_pCommandChart[PLAYER_1]);
	SAFE_DELETE(m_pCommandChart[PLAYER_2]);
}

//-----------------------------------------------------------------------------
// モードをセット
//-----------------------------------------------------------------------------
void CCommandChartManager::SetCommandChartMode(int ID, CCommandChart::MODE_COMMAND_CHART mode)
{
	m_pCommandChart[ID]->SetCommandChartMode(mode);
}
// EOF