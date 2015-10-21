//=============================================================================
//
//	CCommandChartクラスの処理(CCommandChart.cpp)
//
//	Author : 佐藤　諒一
//
//=============================================================================
// インクルード
#include "CCommandChart.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "CCommandChartUI.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
static const float UI_X_POSITION = 150.0f;	// UIの初期表示座標(x座標)
static const float UI_Y_POSITION = 300.0f;	// UIの初期表示座標(y座標)
static const float UI_X_POS_ADD = 50.0f;	// １つ表示後のUIの座標の変化の値
static const int COMMAND_DETH_COUNT = 60;	// コマンド消去までのカウント

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* device)
{
	// デバイスのポインタを保存
	m_pD3DDevice = device;
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
CCommandChart::~CCommandChart()
{
}

//-----------------------------------------------------------------------------
//	初期化処理
//-----------------------------------------------------------------------------
void CCommandChart::Init(void)
{
	// 「入力されたコマンド保持用配列」と「表示する入力されたコマンドUIの保持」を空に
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// 入力されたコマンド保持用配列
		m_aCommandKeep[i] = BUTTON_TYPE_NONE;
		// 表示する入力されたコマンドUIの保持
		m_apCommandUI[i] = NULL;
	}

	// 保持中のコマンド数の初期化
	m_nKeepCommandNum = 0;

	// 入力後のUIを表示するx座標の初期化
	m_fPosX = UI_X_POSITION;

	// コマンド消去カウンターの初期化
	m_nCommandDethCnt = 0;

	// コマンド入力判断フラグの初期化
	m_isCommandInput = true;
}

//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
	// コマンド入力可能
	if (m_isCommandInput)
	{
		// キー入力されたらそれを保持
		// Q
		if (CInputKeyboard::GetKeyboardTrigger(DIK_Q))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = DIK_Q;

			// UIの作成
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create( m_pD3DDevice,
																		BUTTON_TYPE_1,
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;
		}
		// W
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_W))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = DIK_W;

			// UIの作成
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create(	m_pD3DDevice, 
																		BUTTON_TYPE_2, 
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;
		}
		// A
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_A))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = DIK_A;

			// UIの作成
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create(	m_pD3DDevice, 
																		BUTTON_TYPE_3, 
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;
		}
		// S
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_S))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = DIK_S;

			// UIの作成
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create(	m_pD3DDevice, 
																		BUTTON_TYPE_4, 
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;
		}
	}
	else
	{
		// コマンド消去カウンターの増加
		m_nCommandDethCnt++;

		// COMMAND_DETH_COUNTより大きい値になったら行う
		if (m_nCommandDethCnt > COMMAND_DETH_COUNT)
		{
			// 現在表示しているUIをリストから削除して終了処理を行ってからデリートするよ
			for (int i = 0; i < MAX_COMMAND_KEEP; i++)
			{
				m_apCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_UI);
				m_apCommandUI[i]->Uninit();
				SAFE_DELETE(m_apCommandUI[i]);
			}
			// コマンド保持数と保持コマンドのリセット
			Init();
		}
	}

	// 保持中のコマンド数が最大になったら初期化
	if (m_nKeepCommandNum == MAX_COMMAND_KEEP)
	{
		// コマンド入力判定フラグを不可に
		m_isCommandInput = false;
	}
}

//-----------------------------------------------------------------------------
//	描画処理
//-----------------------------------------------------------------------------
void CCommandChart::Draw(void)
{
#ifdef _DEBUG
	// 現在の入力状況をデバックプロシージャで表示
	CDebugProc::Print("現在の入力状況");
	// 入力されている回数回すよ
	for (int i = 0; i < m_nKeepCommandNum; i++)
	{
		// D
		if (m_aCommandKeep[i] == DIK_Q)
		{
			CDebugProc::Print("Q");
		}
		// W
		else if (m_aCommandKeep[i] == DIK_W)
		{
			CDebugProc::Print("W");
		}
		// A
		else if (m_aCommandKeep[i] == DIK_A)
		{
			CDebugProc::Print("A");
		}
		// S
		else if (m_aCommandKeep[i] == DIK_S)
		{
			CDebugProc::Print("S");
		}
	}
	CDebugProc::Print("\n");
#endif
}

//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void CCommandChart::Uninit(void)
{
}

// EOF