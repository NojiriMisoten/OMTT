//=============================================================================
//
//	CCommandChartクラスの処理(CCommandChart.cpp)
//
//	Author : 佐藤　諒一
//
//=============================================================================
// インクルード
#include "CCommandChart.h"
#include "CCommandChartUI.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
static const float UI_X_POSITION = 100.0f;		// UIの初期表示座標(x座標)
static const float UI_Y_POSITION = 300.0f;		// UIの初期表示座標(y座標)
static const float UI_X_POS_ADD = 50.0f;		// １つ表示後のUIの座標の変化の値
static const float NEXT_UI_X_POS = 100.0f;		// 次に入力するコマンドの一番上のUIの座標
static const float NEXT_UI_Y_POS = 300.0f;		// 次に入力するコマンドの一番上のUIの座標
static const float NEXT_UI_X_POS_ADD = 100.0f;	// 次に入力するコマンドのUIの座標の変化の値
static const float NEXT_UI_Y_POS_ADD = 100.0f;	// 次に入力するコマンドのUIの座標の変化の値
static const int COMMAND_DETH_COUNT = 60;		// コマンド消去までのカウント

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber)
{
	// デバイスのポインタを保存
	m_pD3DDevice = pDevice;

	// 「入力されたコマンド保持用配列」と「表示する入力されたコマンドUIの保持」を初期化に
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// 入力されたコマンド保持用配列
		m_aCommandKeep[i] = KEYBOARD_CODE_DECIDE;
		// 表示する入力されたコマンドUIの保持
		m_apCommandUI[i] = NULL;
	}

	// 「次に入力する候補のコマンドUI保持配列」を初期化
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		m_apNextCommandUI[i] = NULL;
	}

	// 自身のプレイヤー番号を設定
	m_MyNumber = PlayerNumber;
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
CCommandChart::~CCommandChart()
{
}

//-----------------------------------------------------------------------------
//	生成処理
//-----------------------------------------------------------------------------
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber)
{
	// コマンドチャートの生成
	CCommandChart* temp = new CCommandChart(pDevice, PlayerNumber);
	// 生成したコマンドチャートの初期化
	temp->Init();

	return temp;
}

//-----------------------------------------------------------------------------
//	初期化処理
//-----------------------------------------------------------------------------
void CCommandChart::Init(void)
{
	// 保持中のコマンド数の初期化
	m_nKeepCommandNum = 0;

	// 入力後のUIを表示するx座標の初期化
	m_fPosX = UI_X_POSITION;

	// コマンド消去カウンターの初期化
	m_nCommandDethCnt = 0;

	// コマンド入力判断フラグの初期化
	m_isCommandInput = true;

	// 最初に入力すべきコマンドの作成
	CreateNextCommand(m_nKeepCommandNum);
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
		if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_COMMAND_DEBUG_Q))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CODE_COMMAND_DEBUG_Q;

			// キー入力情報の更新
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_1)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// 入力されているコマンドを消さないようにフラグを変更
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// 目標座標の設定
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;

			// 入力候補のコマンドを消すよ
			DethNextCommand();

			// 次に入力すべきコマンドの作成
			CreateNextCommand(m_nKeepCommandNum);
		}
		// W
		else if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_COMMAND_DEBUG_W))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CODE_COMMAND_DEBUG_W;

			// キー入力情報の更新
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_2)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// 入力されているコマンドを消さないようにフラグを変更
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// 目標座標の設定
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;

			// 入力候補のコマンドを消すよ
			DethNextCommand();

			// 次に入力すべきコマンドの作成
			CreateNextCommand(m_nKeepCommandNum);
		}
		// A
		else if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_COMMAND_DEBUG_A))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CODE_COMMAND_DEBUG_A;

			// キー入力情報の更新
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_3)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// 入力されているコマンドを消さないようにフラグを変更
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// 目標座標の設定
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;

			// 入力候補のコマンドを消すよ
			DethNextCommand();

			// 次に入力すべきコマンドの作成
			CreateNextCommand(m_nKeepCommandNum);
		}
		// S
		else if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_COMMAND_DEBUG_S))
		{
			// キー入力の保存
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CODE_COMMAND_DEBUG_S;

			// キー入力情報の更新
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_4)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// 入力されているコマンドを消さないようにフラグを変更
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// 目標座標の設定
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// コマンド保持数の増加
			m_nKeepCommandNum++;

			// 描画するx座標の更新
			m_fPosX += UI_X_POS_ADD;

			// 入力候補のコマンドを消すよ
			DethNextCommand();

			// 次に入力すべきコマンドの作成
			CreateNextCommand(m_nKeepCommandNum);
		}
	}
	else
	{
		// コマンド消去までのカウンターの増加
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
	CDebugProc::PrintL("現在の入力状況");
	// 入力されている回数回すよ
	for (int i = 0; i < m_nKeepCommandNum; i++)
	{
		// D
		if (m_aCommandKeep[i] == KEYBOARD_CODE_COMMAND_DEBUG_Q)
		{
			CDebugProc::PrintL("Q");
		}
		// W
		else if (m_aCommandKeep[i] == KEYBOARD_CODE_COMMAND_DEBUG_W)
		{
			CDebugProc::PrintL("W");
		}
		// A
		else if (m_aCommandKeep[i] == KEYBOARD_CODE_COMMAND_DEBUG_A)
		{
			CDebugProc::PrintL("A");
		}
		// S
		else if (m_aCommandKeep[i] == KEYBOARD_CODE_COMMAND_DEBUG_S)
		{
			CDebugProc::PrintL("S");
		}
	}
	CDebugProc::PrintL("\n");
#endif
}

//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void CCommandChart::Uninit(void)
{
}

//-----------------------------------------------------------------------------
//	次に入力する候補のコマンドの作成
//-----------------------------------------------------------------------------
void CCommandChart::CreateNextCommand(int nNumCommand)
{
	// 次に表示するコマンドの生成座標
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// 次に表示するコマンドの目標X座標
	float fPosDestX = 0.0f;

	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyNumber == PLAYER_NUMBER_1)
	{
		fPosDestX = NEXT_UI_X_POS + (NEXT_UI_X_POS_ADD * nNumCommand);
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyNumber == PLAYER_NUMBER_2)
	{
		fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * nNumCommand);
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	// 保持数がMAXで無ければ次のコマンドを表示
	if (m_nKeepCommandNum < MAX_COMMAND_KEEP)
	{
		// 次のコマンドを全作成
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);
			switch (i + 1)
			{
			case BUTTON_TYPE_1:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY,0.0f));
				break;
			case BUTTON_TYPE_2:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			case BUTTON_TYPE_3:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			case BUTTON_TYPE_4:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	次に入力する候補のコマンドの削除
//-----------------------------------------------------------------------------
void CCommandChart::DethNextCommand(void)
{
	// 現在表示しているUIをリストから削除して終了処理を行ってからデリートするよ
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (!m_apNextCommandUI[i]->GetInputFlag())
		{
			m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_UI);
			m_apNextCommandUI[i]->Uninit();
			SAFE_DELETE(m_apNextCommandUI[i]);
		}
	}
}

// EOF