//=============================================================================
//
//	CCommandChartクラスの処理(CCommandChart.cpp)
//
//	Author : 佐藤　諒一
//
//=============================================================================
// インクルード
#include "CCommandChart.h"
#include "CCommandName.h"
#include "../../../CONTROLLER/CControllerManager.h"
#include "../../../INPUT/CInputKeyboard.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
static const float UI_X_POSITION = 50.0f;		// UIの初期表示座標(x座標)
static const float UI_Y_POSITION = 550.0f;		// UIの初期表示座標(y座標)
static const float UI_X_POS_ADD = 30.0f;		// １つ表示後のUIの座標の変化の値
static const float NEXT_UI_X_POS = 50.0f;		// 次に入力するコマンドの一番上のUIの座標
static const float NEXT_UI_Y_POS = 550.0f;		// 次に入力するコマンドの一番上のUIの座標
static const float NEXT_UI_X_POS_ADD = 30.0f;	// 次に入力するコマンドのUIの座標の変化の値
static const float NEXT_UI_Y_POS_ADD = 30.0f;	// 次に入力するコマンドのUIの座標の変化の値
static const int COMMAND_DEATH_COUNT = 60;		// コマンド消去までのカウント
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//フェードアウト時の目標座標自分のID１
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//フェードアウト時の目標座標自分のID２
static const float BACK_POLYGON_X_SIZE = (COMMAND_POLYGON_WIDTH*MAX_COMMAND_KEEP) + (UI_X_POS_ADD*MAX_COMMAND_KEEP);	// コマンドチャートUIの背後に表示するポリゴンのXのサイズ
static const float BACK_POLYGON_Y_SIZE = 150.0f;	// コマンドチャートUIの背後に表示するポリゴンのYのサイズ
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標ID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標ID2
// コマンドの種類


//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID)
{
	// デバイスのポインタを保存
	m_pD3DDevice = pDevice;

	// 「入力されたコマンド保持用配列」と「表示する入力されたコマンドUIの保持」を初期化に
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// 入力されたコマンド保持用配列
		m_aCommandKeep[i] = BUTTON_TYPE_NONE;
		// 表示する入力されたコマンドUIの保持
		m_apCommandUI[i] = NULL;
	}

	// 「次に入力する候補のコマンドUI保持配列」と「発生候補の技名表示用UI保持配列」を初期化
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		m_apNextCommandUI[i] = NULL;
		m_apCommandName[i] = NULL;
	}

	// 自身のプレイヤー番号を設定
	m_MyID = nID;

	// 背後に表示するポリゴンの作成
	m_pBackPolygon = new CScene2D(pDevice);
	if (m_MyID == MY_ID_1)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_1, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_MONO);
	}
	else if (m_MyID == MY_ID_2)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_2, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_MONO);
	}
	m_pBackPolygon->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pBackPolygon->SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f));
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
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, int nID)
{
	// コマンドチャートの生成
	CCommandChart* temp = new CCommandChart(pDevice, nID);
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
	m_nCommandDeathCnt = 0;

	// コマンド入力判断フラグの初期化
	m_isCommandInput = true;

	// 最初に入力すべきコマンドの作成
	CreateNextCommand(m_nKeepCommandNum);

	D3DXVECTOR3 pos;


	// 技名表示用UIの初期座標の設定
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_MyID == MY_ID_1)
		{
			pos = D3DXVECTOR3(UI_X_POSITION + UI_X_POS_ADD + UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}
		else if (m_MyID == MY_ID_2)
		{
			pos = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}

		// 発生候補の技名表示用UIを作成
		m_apCommandName[i] = CCommandName::Create(m_pD3DDevice, pos, TEXTURE_MONO);
	}
}

//-----------------------------------------------------------------------------
//	デフォルトの状態に戻す処理
//-----------------------------------------------------------------------------
void CCommandChart::SetDefault(void)
{
	// 保持中のコマンド数の初期化
	m_nKeepCommandNum = 0;

	// 入力後のUIを表示するx座標の初期化
	m_fPosX = UI_X_POSITION;

	// コマンド消去カウンターの初期化
	m_nCommandDeathCnt = 0;

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
		// コマンド入力
		InputCommand();
	}
	// コマンド入力不可
	else
	{
		// コマンドのリセット
		ResetCommand();
	}

	// 保持中のコマンド数が最大になったら初期化するために一旦入力禁止
	if (m_nKeepCommandNum >= MAX_COMMAND_KEEP)
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
}

//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void CCommandChart::Uninit(void)
{
	m_pD3DDevice = NULL;
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
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS + (NEXT_UI_X_POS_ADD * nNumCommand);
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
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
				// Qもしくは右側の上ボタンに対応
			case BUTTON_TYPE_1:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Wもしくは右側の下ボタンに対応
			case BUTTON_TYPE_2:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Aもしくは左側の上ボタンに対応
			case BUTTON_TYPE_3:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Sもしくは右側の下ボタンに対応
			case BUTTON_TYPE_4:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
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
void CCommandChart::DeathNextCommand(void)
{
	// 現在表示しているUIをリストから削除して終了処理を行ってからデリートするよ
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (!m_apNextCommandUI[i]->GetInputFlag())
		{
			m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
			m_apNextCommandUI[i]->Uninit();
			SAFE_DELETE(m_apNextCommandUI[i]);
		}
	}
}

//-----------------------------------------------------------------------------
//	画面外へのフェードアウト
//-----------------------------------------------------------------------------
void CCommandChart::ScreenOut(void)
{
	// 保持中のUIの移動設定から始める
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// 保持配列要素がNULLになったら抜ける
		if (!m_apCommandUI[i])
		{
			break;
		}
		// 自分のIDが１の時の移動処理
		if (m_MyID == MY_ID_1)
		{
			m_apCommandUI[i]->SetDestPos(D3DXVECTOR3(FADE_UI_OUT_POS_X_ID_1, UI_Y_POSITION, 0.0f));
		}
		// 自分のIDが２の時の移動処理
		else if (m_MyID == MY_ID_2)
		{
			m_apCommandUI[i]->SetDestPos(D3DXVECTOR3(FADE_UI_OUT_POS_X_ID_2, UI_Y_POSITION, 0.0f));
		}
	}
	// 入力候補のUIの移動設定
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		// 自分のIDが１の時の移動処理
		if (m_MyID == MY_ID_1)
		{
			m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(FADE_UI_OUT_POS_X_ID_1, UI_Y_POSITION, 0.0f));
		}
		// 自分のIDが２の時の移動処理
		else if (m_MyID == MY_ID_2)
		{
			m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(FADE_UI_OUT_POS_X_ID_2, UI_Y_POSITION, 0.0f));
		}
	}
	// 発動候補の技名のUIの移動設定
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apCommandName[i])
		{
			// 自分のIDが１の時の移動処理
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(FADE_UI_OUT_POS_X_ID_1, UI_Y_POSITION, 0.0f));
			}
			// 自分のIDが２の時の移動処理
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(FADE_UI_OUT_POS_X_ID_2, UI_Y_POSITION, 0.0f));
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	画面内へのフェードイン
//-----------------------------------------------------------------------------
void CCommandChart::ScreenIn(void)
{
}

//-----------------------------------------------------------------------------
//	コマンド入力
//-----------------------------------------------------------------------------
void CCommandChart::InputCommand(void)
{
	bool isPushButton1 = false;
	bool isPushButton2 = false;
	bool isPushButton3 = false;
	bool isPushButton4 = false;
	//キー入力
	if (m_MyID == MY_ID_1)
	{
		// ボタンタイプ1
		// 右上
		//bool isPushButton1 =
		isPushButton1 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_UP) ||			// キーボード入力
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_UP, m_MyID);	// コントローラー入力
		// ボタンタイプ2
		// 右下
		//bool isPushButton2 =
		isPushButton2 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN) ||			// キーボード入力 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_DOWN, m_MyID);// コントローラー入力 
		// ボタンタイプ3
		// 左上
		//bool isPushButton3 =
		isPushButton3 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_UP) ||			// キーボード入力 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_UP, m_MyID);	// コントローラー入力 
		// ボタンタイプ4
		// 左下
		//bool isPushButton4 =
		isPushButton4 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_DOWN) ||			// キーボード入力 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_DOWN, m_MyID);	// コントローラー入力 
		// キー入力されたらそれを保持
	}

	// ボタンタイプ１
	if (isPushButton1)
	{
		// キー入力の保存
		m_aCommandKeep[m_nKeepCommandNum] = BUTTON_TYPE_1;

		// キー入力情報の更新
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_1)
			{
				m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
			}
		}
	}
	// ボタンタイプ２
	else if (isPushButton2)
	{
		// キー入力の保存
		m_aCommandKeep[m_nKeepCommandNum] = BUTTON_TYPE_2;

		// キー入力情報の更新
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_2)
			{
				m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
			}
		}
	}
	// ボタンタイプ３
	else if (isPushButton3)
	{
		// キー入力の保存
		m_aCommandKeep[m_nKeepCommandNum] = BUTTON_TYPE_3;

		// キー入力情報の更新
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_3)
			{
				m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
			}
		}
	}
	// ボタンタイプ４
	else if (isPushButton4)
	{
		// キー入力の保存
		m_aCommandKeep[m_nKeepCommandNum] = BUTTON_TYPE_4;

		// キー入力情報の更新
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_4)
			{
				m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
			}
		}
	}
	// 何か入力されていた場合の共通処理
	if (isPushButton1 || isPushButton2 || isPushButton3 || isPushButton4)
	{
		// 入力されているコマンドを消さないようにフラグを変更
		m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

		// コマンドUIの目標座標の設定
		if (m_MyID == MY_ID_1)
		{
			m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
		}
		else if (m_MyID == MY_ID_2)
		{
			m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
		}

		// 発生候補の技名表示用UIの目標座標の設定
		// 技名表示用UIの初期座標の設定
		if (m_nKeepCommandNum < MAX_COMMAND_KEEP - 1)
		{
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_MyID == MY_ID_1)
				{
					m_apCommandName[i]->SetDestPos(D3DXVECTOR3(m_fPosX + UI_X_POS_ADD + UI_X_POS_ADD + UI_X_POSITION, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
				}
				else if (m_MyID == MY_ID_2)
				{
					m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POS_ADD - UI_X_POSITION - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
				}
			}
		}

		// コマンド保持数の増加
		m_nKeepCommandNum++;

		// 描画するx座標の更新
		m_fPosX += UI_X_POS_ADD;

		// 入力候補のコマンドを消すよ
		DeathNextCommand();

		// 次に入力すべきコマンドの作成
		CreateNextCommand(m_nKeepCommandNum);
	}
}

//-----------------------------------------------------------------------------
//	コマンドのリセット
//-----------------------------------------------------------------------------
void CCommandChart::ResetCommand(void)
{
	// コマンド消去までのカウンターの増加
	m_nCommandDeathCnt++;

	// COMMAND_DETH_COUNTより大きい値になったら行う
	if (m_nCommandDeathCnt > COMMAND_DEATH_COUNT)
	{
		// 現在表示しているUIをリストから削除して終了処理を行ってからデリートするよ
		for (int i = 0; i < MAX_COMMAND_KEEP; i++)
		{
			m_apCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
			m_apCommandUI[i]->Uninit();
			SAFE_DELETE(m_apCommandUI[i]);
		}
		// コマンド保持数と保持コマンドのリセット
		SetDefault();
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + UI_X_POS_ADD + UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
	}
}

// EOF