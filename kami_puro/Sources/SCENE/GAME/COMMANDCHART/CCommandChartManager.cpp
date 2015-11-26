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
CCommandChartManager::CCommandChartManager()
{
	// コマンドチャートクラス格納用のポインタをNULLに
	m_pCommandChart[MY_ID_1] = NULL;
	m_pCommandChart[MY_ID_2] = NULL;
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
CCommandChartManager* CCommandChartManager::Create(LPDIRECT3DDEVICE9* pDevice)
{
	CCommandChartManager* temp = new CCommandChartManager;
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
	m_pCommandChart[MY_ID_1] = CCommandChart::Create(pDevice, MY_ID_1, this);
	m_pCommandChart[MY_ID_2] = CCommandChart::Create(pDevice, MY_ID_2, this);
}

//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Update(void)
{
	// コマンドチャートの更新
	m_pCommandChart[MY_ID_1]->Update();
	m_pCommandChart[MY_ID_2]->Update();

	Draw();
}

//-----------------------------------------------------------------------------
//	描画処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Draw(void)
{
	// コマンドチャートの描画
	m_pCommandChart[MY_ID_1]->Draw();
	m_pCommandChart[MY_ID_2]->Draw();
}

//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void CCommandChartManager::Uninit(void)
{
	// コマンドチャートの終了
	m_pCommandChart[MY_ID_1]->Uninit();
	m_pCommandChart[MY_ID_2]->Uninit();
	
	// 後片付け
	SAFE_DELETE(m_pCommandChart[MY_ID_1]);
	SAFE_DELETE(m_pCommandChart[MY_ID_2]);
}

//-----------------------------------------------------------------------------
// モードをセット
//-----------------------------------------------------------------------------
void CCommandChartManager::SetCommandChartMode(int ID, CCommandChart::MODE_COMMAND_CHART mode)
{
	m_pCommandChart[ID]->SetCommandChartMode(mode);
}
// EOF