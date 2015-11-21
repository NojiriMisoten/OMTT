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
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//フェードアウト時の目標座標自分のID１
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//フェードアウト時の目標座標自分のID２
static const float BACK_POLYGON_X_SIZE = (COMMAND_POLYGON_WIDTH*MAX_COMMAND_KEEP) + (UI_X_POS_ADD*MAX_COMMAND_KEEP);	// コマンドチャートUIの背後に表示するポリゴンのXのサイズ
static const float BACK_POLYGON_Y_SIZE = 150.0f;	// コマンドチャートUIの背後に表示するポリゴンのYのサイズ
static const int COMMAND_DEATH_COUNT = 60;		// コマンド消去までのカウント
static const int COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM = 0;	// 弱攻撃のコマンドの配列番号
static const int COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM = 1;	// 中攻撃のコマンドの配列番号
static const int COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM = 2;	// 弱攻撃のコマンドの配列番号
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標ID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標ID2

// コマンドの種類
// チョップ LU LU RU
static const BUTTON_TYPE COMMAND_BUTTON_CHOP[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_3,
																   BUTTON_TYPE_3,
																   BUTTON_TYPE_1 };
static const COMMAND_INFO COMMAND_CHOP = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_CHOP, COMMAND_BUTTON_CHOP };

//エルボー	LU LD RU RD
static const BUTTON_TYPE COMMAND_BUTTON_ELBOW[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_3,
																	 BUTTON_TYPE_4,
																	 BUTTON_TYPE_1,
																	 BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_ELBOW = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_ELBOW, COMMAND_BUTTON_ELBOW };

//ラリアット	LU LU LU RD LD
static const BUTTON_TYPE COMMAND_BUTTON_LARIAT[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_3,
																	 BUTTON_TYPE_3,
																	 BUTTON_TYPE_3,
																	 BUTTON_TYPE_2,
																	 BUTTON_TYPE_4 };
static const COMMAND_INFO COMMAND_LARIAT = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_LARIAT, COMMAND_BUTTON_LARIAT };

//ローリングエルボー	RU LU LD
static const BUTTON_TYPE COMMAND_BUTTON_ROLLING_ELBOW[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_3,
																			BUTTON_TYPE_1,
																			BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_ROLLING_ELBOW = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_ROLLING_ELBOW, COMMAND_BUTTON_ROLLING_ELBOW };

//フライングエルボー	RU RU LU RD
static const BUTTON_TYPE COMMAND_BUTTON_FLYING_ELBOW[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_3,
																			BUTTON_TYPE_3, 
																			BUTTON_TYPE_1,
																			BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_FLYING_ELBOW = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_FLYING_ELBOW, COMMAND_BUTTON_FLYING_ELBOW };

//ドロップキック		RU LU RU RU RD
static const BUTTON_TYPE COMMAND_BUTTON_DROP_KICK[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_3,
																		BUTTON_TYPE_1,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_4 };
static const COMMAND_INFO COMMAND_DROP_KICK = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_DROP_KICK, COMMAND_BUTTON_DROP_KICK };

//ビンタ			LD LU RU
static const BUTTON_TYPE COMMAND_BUTTON_FACE_SLAPPING[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_4,
																			BUTTON_TYPE_3,
																			BUTTON_TYPE_1 };
static const COMMAND_INFO COMMAND_FACE_SLAPPING = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_FACE_SLAPPING, COMMAND_BUTTON_FACE_SLAPPING };

//バックドロップ		LD LD RU RD
static const BUTTON_TYPE COMMAND_BUTTON_BACKDROP[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_4,
																		BUTTON_TYPE_4,
																		BUTTON_TYPE_1,
																		BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_BACKDROP = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_BACKDROP, COMMAND_BUTTON_BACKDROP };

//スタナー		LD RU RU LU RD
static const BUTTON_TYPE COMMAND_BUTTON_STANER[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_4,
																	 BUTTON_TYPE_1,
																	 BUTTON_TYPE_1,
																	 BUTTON_TYPE_3,
																	 BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_STANER = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_STANER, COMMAND_BUTTON_STANER };

//ロープ	RD
static const BUTTON_TYPE COMMAND_BUTTON_ROPE = BUTTON_TYPE_2;
static const COMMAND_INFO COMMAND_ROPE = { COMMAND_INPUT_NUM_MONO, COMMAND_TYPE_ROPE, (BUTTON_TYPE*)&COMMAND_BUTTON_ROPE };

//Finish　LU + RU RU LU RU LU LD + RD
static const BUTTON_TYPE COMMAND_BUTTON_FINISHER[COMMAND_INPUT_NUM_FINISHER] = { BUTTON_TYPE_5,
																	      BUTTON_TYPE_1,
																	      BUTTON_TYPE_3,
																	      BUTTON_TYPE_1,
																	      BUTTON_TYPE_3,
																	      BUTTON_TYPE_6 };
static const COMMAND_INFO COMMAND_FINISHER = { COMMAND_INPUT_NUM_FINISHER, COMMAND_TYPE_FINISHER, COMMAND_BUTTON_FINISHER };


// 技の大きさでまとめた配列
// 小技
// チョップ
// ローリングエルボー
// ビンタ
static const COMMAND_INFO* COMMAND_SMALL_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_CHOP,
																			  &COMMAND_ROLLING_ELBOW,
																			  &COMMAND_FACE_SLAPPING};
// 中技
// エルボー
// フライングエルボー
// バックドロップ
static const COMMAND_INFO* COMMAND_MIDDLE_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_ELBOW,
																			  &COMMAND_FLYING_ELBOW,
																			  &COMMAND_BACKDROP };

// 大技
// ラリアット
// ドロップキック
// スタナー
static const COMMAND_INFO* COMMAND_LARGE_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_LARIAT,
																			  &COMMAND_DROP_KICK,
																			  &COMMAND_STANER };

// 左上キー開始コマンドをまとめた配列
// チョップ
// エルボー
// ラリアット
static const COMMAND_INFO* COMMAND_LEFT_UP_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = {&COMMAND_CHOP,
																				&COMMAND_ELBOW,
																				&COMMAND_LARIAT};

// 右上キー開始コマンドをまとめた配列
// ローリングエルボー
// フライングエルボー
// ドロップキック
static const COMMAND_INFO* COMMAND_RIGHT_UP_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_ROLLING_ELBOW,
																				&COMMAND_FLYING_ELBOW,
																				&COMMAND_DROP_KICK};

// 左下キー開始コマンドをまとめた配列
// ビンタ
// バックドロップ
// スタナー
static const COMMAND_INFO* COMMAND_LEFT_DOWN_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_FACE_SLAPPING,
																					&COMMAND_BACKDROP,
																					&COMMAND_STANER };

// 全ての技をまとめた配列
static const COMMAND_INFO* COMMAND_TECHNIQUE_LIST[COMMAND_TYPE_MAX] = { 
&COMMAND_CHOP,
&COMMAND_ELBOW,
&COMMAND_LARIAT,
&COMMAND_ROLLING_ELBOW,
&COMMAND_FLYING_ELBOW,
&COMMAND_DROP_KICK,
&COMMAND_FACE_SLAPPING,
&COMMAND_BACKDROP,
&COMMAND_STANER,
&COMMAND_ROPE,
&COMMAND_FINISHER};

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID)
{
	// デバイスのポインタを保存
	m_pD3DDevice = pDevice;

	// 入力されたコマンド保持用変数の初期化
	m_aCommandKeep = BUTTON_TYPE_NONE;

	// 「入力されたコマンド保持用配列」と「表示する入力されたコマンドUIの保持」を初期化に
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// 表示する入力されたコマンドUIの保持
		//m_apCommandUI[i] = NULL;
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

	// コマンド入力情報のポインタの初期化
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		m_pCommandUI[i] = new COMMAND_UI_INFO;
		m_pCommandUI[i]->m_isInputButton = false;
		m_pCommandUI[i]->m_NextCommand = NULL;
		m_pCommandUI[i]->m_pUIType = NULL;
	}
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
	CreateFirstCommand();

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
	CreateFirstCommand();

	// コマンド入力情報のポインタの初期化
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		m_pCommandUI[i]->m_isInputButton = false;
		m_pCommandUI[i]->m_NextCommand = NULL;
		m_pCommandUI[i]->m_pUIType = NULL;
	}
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

	if (UseTechnic() != COMMAND_TYPE_NONE)
	{
		// コマンド入力判定フラグを不可に
		m_isCommandInput = false;
		// コマンドのリセット
		//ResetCommand();
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

	// 自分のコマンドUIをリストから削除して終了処理を行ってからデリートするよ
	COMMAND_UI_INFO* pCommandUITemp = NULL;
	COMMAND_UI_INFO* pCommandNextUITemp = NULL;
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		pCommandUITemp = m_pCommandUI[i];

		while (pCommandUITemp)
		{
			pCommandNextUITemp = pCommandUITemp->m_NextCommand;
			if (pCommandUITemp->m_pUIType)
			{
				pCommandUITemp->m_pUIType->Uninit();
			}
			SAFE_DELETE(pCommandUITemp);
			pCommandUITemp = pCommandNextUITemp;
		}
	}
	// 現在表示しているUIをリストから削除して終了処理を行うよ
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apNextCommandUI[i])
		{
			m_apNextCommandUI[i]->Uninit();
		}
	}
}

//-----------------------------------------------------------------------------
//	最初に入力する候補のコマンドの作成
//-----------------------------------------------------------------------------
void CCommandChart::CreateFirstCommand(void)
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
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS);
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	// 保持数がMAXで無ければ次のコマンドを表示
	if (m_nKeepCommandNum < MAX_COMMAND_KEEP)
	{
		// 次のコマンドを全作成
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			// 初期Y座標
			fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

			// 各初期ボタンの生成
			switch (i + 1)
			{
				// Qもしくは右側の上ボタンに対応
			case BUTTON_TYPE_1:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				break;
				// Wもしくは右側の下ボタンに対応
			case BUTTON_TYPE_2:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				break;
				// Aもしくは左側の上ボタンに対応
			case BUTTON_TYPE_3:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				break;
				// Sもしくは右側の下ボタンに対応
			case BUTTON_TYPE_4:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				break;
			default:
				break;
			}
			// 生成後目指す座標の設定
			m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
		}
	}
}

//-----------------------------------------------------------------------------
//	最初に入力する候補のコマンドの再開始
//-----------------------------------------------------------------------------
void CCommandChart::RestartFirstCommandUI(void)
{
	// 現在表示しているUIをリストから削除して終了処理を行うよ
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apNextCommandUI[i])
		{
			m_apNextCommandUI[i]->SetDrawFlag(true);
		}
	}
}

//-----------------------------------------------------------------------------
//	次に入力する候補のコマンドの作成
//-----------------------------------------------------------------------------
void CCommandChart::CreateNextCommand(void)
{
	switch (m_aCommandKeep)
	{
		// Qもしくは右側の上ボタンに対応
	case BUTTON_TYPE_1:
		CreateRightUpTechnicCommand();
		break;
		// Wもしくは右側の下ボタンに対応
	case BUTTON_TYPE_2:
		break;
		// Aもしくは左側の上ボタンに対応
	case BUTTON_TYPE_3:
		CreateLeftUpTechnicCommand();
		break;
		// Sもしくは右側の下ボタンに対応
	case BUTTON_TYPE_4:
		CreateLeftDownTechnicCommand();
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//	次に入力する候補のコマンドの削除
//-----------------------------------------------------------------------------
void CCommandChart::DeathNextCommand(void)
{
	// 現在表示しているUIをリストから削除して終了処理を行うよ
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apNextCommandUI[i])
		{
			if (!m_apNextCommandUI[i]->GetInputFlag())
			{
				m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	画面外へのフェードアウト
//-----------------------------------------------------------------------------
void CCommandChart::ScreenOut(void)
{
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
	else if (m_MyID == MY_ID_2)
	{
		// ボタンタイプ1
		// 右上
		//bool isPushButton1 =
		isPushButton1 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_UP) ||			// キーボード入力
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_UP, m_MyID);	// コントローラー入力
		// ボタンタイプ2
		// 右下
		//bool isPushButton2 =
		isPushButton2 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN) ||			// キーボード入力 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_DOWN, m_MyID);// コントローラー入力 
		// ボタンタイプ3
		// 左上
		//bool isPushButton3 =
		isPushButton3 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_UP) ||			// キーボード入力 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_UP, m_MyID);	// コントローラー入力 
		// ボタンタイプ4
		// 左下
		//bool isPushButton4 =
		isPushButton4 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_DOWN) ||			// キーボード入力 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_DOWN, m_MyID);	// コントローラー入力 
		// キー入力されたらそれを保持
	}

	// ボタンタイプ１
	if (isPushButton1)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_1;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_1 - 1]->SetInputFlag(true);
		}
	}
	// ボタンタイプ２
	else if (isPushButton2)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_2;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_2 - 1]->SetInputFlag(true);
		}
	}
	// ボタンタイプ３
	else if (isPushButton3)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_3;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_3 - 1]->SetInputFlag(true);
		}
	}
	// ボタンタイプ４
	else if (isPushButton4)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_4;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_4 - 1]->SetInputFlag(true);
		}
	}
	// 何か入力されていた場合の共通処理
	if (isPushButton1 || isPushButton2 || isPushButton3 || isPushButton4)
	{
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

		// 最初のコマンド入力の時の処理
		if (m_nKeepCommandNum == 1)
		{
			// 入力候補のコマンドを消すよ
			DeathNextCommand();

			// 次に入力すべきコマンドの作成
			CreateNextCommand();
		}
		// 2回目からのコマンドの入力の時の処理
		else
		{
			// コマンドの入力チェック
			CommandUIInput(m_aCommandKeep);
		}

		// コマンド入力からコマンド初期化までの間の初期化
		m_nCommandDeathCnt = 0;
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
	//if (m_nCommandDeathCnt > COMMAND_DEATH_COUNT)
	{
		// 自分のコマンドUIをリストから削除して終了処理を行ってからデリートするよ
		for (int i = 0; i < COMMAND_TYPE_NUM; i++)
		{
			COMMAND_UI_INFO* pCommandUITemp = m_pCommandUI[i];

			while (pCommandUITemp)
			{
				COMMAND_UI_INFO* pCommandNextUITemp = pCommandUITemp->m_NextCommand;
				if (pCommandUITemp->m_pUIType)
				{
					pCommandUITemp->m_pUIType->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
					pCommandUITemp->m_NextCommand = NULL;
					pCommandUITemp->m_pUIType = NULL;
					pCommandUITemp->m_isInputButton = false;
				}
				pCommandUITemp = pCommandNextUITemp;
			}
		}

		// 現在表示しているUIをリストから削除して終了処理を行うよ
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_apNextCommandUI[i])
			{
				m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
			}
		}

		// 「入力されたコマンド保持用変数の初期化
		// 入力されたコマンド保持用
		m_aCommandKeep = BUTTON_TYPE_NONE;

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

//-----------------------------------------------------------------------------
// コマンドのチェック
// 引数：技候補のアドレス
// 戻り値：コマンドが正しかったか否か
//-----------------------------------------------------------------------------
bool CCommandChart::CheckCommand(COMMAND_UI_INFO* Technic, COMMAND_INFO* pCommand)
{
	bool isMatch = false;	// 一致していたか
	int nMatchNum = 0;		// コマンドの数確認

	/*
	// 入力されているコマンドと送られてきたコマンドが一致しているか確認
	for (int i = 0; i < m_nKeepCommandNum; i++)
	{
		if (m_aCommandKeep == Technic->m_Command[i])
		{
			// 一致の数を増やす
			nMatchNum++;
		}
	}

	// 一致数と入力数が同じならば一致しているとする
	if (nMatchNum == m_nKeepCommandNum)
	{
		isMatch = true;
	}
	*/

	// 渡されたリストを最後まで確認
	while (1)
	{
		// 現在の入力候補のコマンドがNULLだった時はループを抜ける
		if (!Technic->m_pUIType)
		{
			break;
		}

		// 現在入力候補のコマンドのボタンが押されていたら
		if (Technic->m_isInputButton)
		{
			if (!Technic->m_pUIType)
			{
				break;
			}
			// コマンドの比較
			if (Technic->m_pUIType->GetButtonType() == pCommand->m_Command[nMatchNum])
			{
				// もし次の候補のコマンドのボタンが無かった場合このコマンドは成立している
				if (!Technic->m_NextCommand->m_NextCommand)
				{
					isMatch = true;
					break;
				}
				// 次の候補のコマンドがあった場合は次の候補にポインタをずらす
				else
				{
					Technic = Technic->m_NextCommand;
				}
				nMatchNum++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	// 結果を返す
	return isMatch;
}

//-----------------------------------------------------------------------------
// 技の確定
// 戻り値：繰り出す技
//
// コマンドが長いものの方が優先される
//-----------------------------------------------------------------------------
COMMAND_TYPE CCommandChart::UseTechnic(void)
{
	bool isUseTechnic = false;
	COMMAND_TYPE CommandType = COMMAND_TYPE_NONE;	// 最終的に返すべき技
	
	// (小技)
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		if (!m_pCommandUI[COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			break;
		}

		// 入力されているコマンドがどの技と一致しているか確認
		// 一致していればture一致していなければfalseをisUseTechnicに入れる
		if (m_pCommandUI[COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			isUseTechnic = CheckCommand(m_pCommandUI[COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM],
				(COMMAND_INFO*)COMMAND_SMALL_TECHNIQUE_LIST[i]);
		}

		// 一致しているコマンドがあるか確認
		if (isUseTechnic)
		{
			// 一致していた場合のコマンドのタイプを返す
			CommandType = COMMAND_SMALL_TECHNIQUE_LIST[i]->m_CommandType;
			break;
		}
	}
	
	// (中技)
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		if (!m_pCommandUI[COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			break;
		}
		// 入力されているコマンドがどの技と一致しているか確認
		// 一致していればture一致していなければfalseをisUseTechnicに入れる
		if (m_pCommandUI[COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			isUseTechnic = CheckCommand(m_pCommandUI[COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM],
				(COMMAND_INFO*)COMMAND_MIDDLE_TECHNIQUE_LIST[i]);
		}

		// 一致しているコマンドがあるか確認
		if (isUseTechnic)
		{
			// 一致していた場合のコマンドのタイプを返す
			CommandType = COMMAND_MIDDLE_TECHNIQUE_LIST[i]->m_CommandType;
			break;
		}
	}
	
	// (大技)
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		if (!m_pCommandUI[COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			break;
		}

		// 入力されているコマンドがどの技と一致しているか確認
		// 一致していればture一致していなければfalseをisUseTechnicに入れる
		if (m_pCommandUI[COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			isUseTechnic = CheckCommand(m_pCommandUI[COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM],
				(COMMAND_INFO*)COMMAND_LARGE_TECHNIQUE_LIST[i]);
		}

		// 一致しているコマンドがあるか確認
		if (isUseTechnic)
		{
			// 一致していた場合のコマンドのタイプを返す
			CommandType = COMMAND_LARGE_TECHNIQUE_LIST[i]->m_CommandType;
			break;
		}
	}

	// ロープコマンド
	if (m_apNextCommandUI[BUTTON_TYPE_2 - 1])
	{
		if (m_apNextCommandUI[BUTTON_TYPE_2 - 1]->GetInputFlag())
		{
			CommandType = COMMAND_TYPE_ROPE;
		}
	}

	// 決定した技を返す
	return CommandType;
}

//-----------------------------------------------------------------------------
// 右上キー開始コマンドの生成
// 引数：何個目のコマンドか
//-----------------------------------------------------------------------------
void CCommandChart::CreateRightUpTechnicCommand(void)
{
	// 次に表示するコマンドの生成座標
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// 次に表示するコマンドの目標X座標
	float fPosDestX = 0.0f;

	// コマンドUI作成時にコマンドUIのポインタを入れるためのポインタ
	COMMAND_UI_INFO* pCommandUI = NULL;

	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// コマンドを表記するためのY座標の設定
		fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

		// 弱中強攻撃のコマンドのリストの先頭アドレスを代入
		pCommandUI = m_pCommandUI[i];

		for (int j = 0; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[i]->m_nCommandLength; j++)
		{
			// 目標の座標
			// プレイヤー１の時の表示X座標
			if (m_MyID == MY_ID_1)
			{
				fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
				fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
			}
			// プレイヤー２の時の表示X座標
			else if (m_MyID == MY_ID_2)
			{
				fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
				fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
			}

			// コマンドUIのボタンの種類に応じたUIを作成する
			switch (COMMAND_RIGHT_UP_TECHNIQUE_LIST[i]->m_Command[j])
			{
				// Qもしくは右側の上ボタンに対応
			case BUTTON_TYPE_1:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Wもしくは右側の下ボタンに対応
			case BUTTON_TYPE_2:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Aもしくは左側の上ボタンに対応
			case BUTTON_TYPE_3:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Sもしくは右側の下ボタンに対応
			case BUTTON_TYPE_4:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
			// 作成対象をリストの次のポインタへつずらす
			pCommandUI->m_NextCommand = new COMMAND_UI_INFO;
			pCommandUI = pCommandUI->m_NextCommand;
			// 次のポインタの初期化
			pCommandUI->m_NextCommand = NULL;
			pCommandUI->m_isInputButton = false;
			pCommandUI->m_pUIType = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 左上キー開始のコマンドの生成
// 引数：何個目のコマンドか
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftUpTechnicCommand(void)
{
	// 次に表示するコマンドの生成座標
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// 次に表示するコマンドの目標X座標
	float fPosDestX = 0.0f;

	// コマンドUI作成時にコマンドUIのポインタを入れるためのポインタ
	COMMAND_UI_INFO* pCommandUI = NULL;

	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// コマンドを表記するためのY座標の設定
		fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

		// 弱中強攻撃のコマンドのリストの先頭アドレスを代入
		pCommandUI = m_pCommandUI[i];

		// コマンドの長さ分回す
		for (int j = 0; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[i]->m_nCommandLength; j++)
		{
			// 目標の座標
			// プレイヤー１の時の表示X座標
			if (m_MyID == MY_ID_1)
			{
				fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
				fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
			}
			// プレイヤー２の時の表示X座標
			else if (m_MyID == MY_ID_2)
			{
				fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
				fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
			}

			// コマンドUIのボタンの種類に応じたUIを作成する
			switch (COMMAND_LEFT_UP_TECHNIQUE_LIST[i]->m_Command[j])
			{
				// Qもしくは右側の上ボタンに対応
			case BUTTON_TYPE_1:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Wもしくは右側の下ボタンに対応
			case BUTTON_TYPE_2:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Aもしくは左側の上ボタンに対応
			case BUTTON_TYPE_3:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Sもしくは右側の下ボタンに対応
			case BUTTON_TYPE_4:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
			// 作成対象をリストの次のポインタへつずらす
			pCommandUI->m_NextCommand = new COMMAND_UI_INFO;
			pCommandUI = pCommandUI->m_NextCommand;
			// 次のポインタの初期化
			pCommandUI->m_NextCommand = NULL;
			pCommandUI->m_isInputButton = false;
			pCommandUI->m_pUIType = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 左下キー開始コマンドの生成
// 引数：何個目のコマンドか
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftDownTechnicCommand(void)
{
	// 次に表示するコマンドの生成座標
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// 次に表示するコマンドの目標X座標
	float fPosDestX = 0.0f;

	// コマンドUI作成時にコマンドUIのポインタを入れるためのポインタ
	COMMAND_UI_INFO* pCommandUI = NULL;

	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// コマンドを表記するためのY座標の設定
		fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

		// 弱中強攻撃のコマンドのリストの先頭アドレスを代入
		pCommandUI = m_pCommandUI[i];

		for (int j = 0; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[i]->m_nCommandLength; j++)
		{
			// 目標の座標
			// プレイヤー１の時の表示X座標
			if (m_MyID == MY_ID_1)
			{
				fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
				fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
			}
			// プレイヤー２の時の表示X座標
			else if (m_MyID == MY_ID_2)
			{
				fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
				fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
			}

			// コマンドUIのボタンの種類に応じたUIを作成する
			switch (COMMAND_LEFT_DOWN_TECHNIQUE_LIST[i]->m_Command[j])
			{
				// Qもしくは右側の上ボタンに対応
			case BUTTON_TYPE_1:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				// コマンドのボタンの種類の確定
				break;
				// Wもしくは右側の下ボタンに対応
			case BUTTON_TYPE_2:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Aもしくは左側の上ボタンに対応
			case BUTTON_TYPE_3:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// Sもしくは右側の下ボタンに対応
			case BUTTON_TYPE_4:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// 生成位置
					TEXTURE_BUTTON);
				// 生成後目指す座標の設定
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
			// 作成対象をリストの次のポインタへつずらす
			pCommandUI->m_NextCommand = new COMMAND_UI_INFO;
			pCommandUI = pCommandUI->m_NextCommand;
			// 次のポインタの初期化
			pCommandUI->m_NextCommand = NULL;
			pCommandUI->m_isInputButton = false;
			pCommandUI->m_pUIType = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	コマンドUIの入力
//-----------------------------------------------------------------------------
void CCommandChart::CommandUIInput(BUTTON_TYPE button)
{
	COMMAND_UI_INFO* pCommandTemp;

	// コマンドの強中弱のリストを調べる
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// 仮変数に代入
		pCommandTemp = m_pCommandUI[i];

		while (1)
		{
			// 現在の入力候補のコマンドがNULLだった時はループを抜ける
			if (!pCommandTemp->m_pUIType)
			{
				break;
			}

			// 現在入力候補のコマンドのボタンの種類が入力されたものと同じだった時
			if (pCommandTemp->m_pUIType->GetButtonType() == button)
			{
				// コマンドの入力がまだ行われていなかった場合コマンドの入力onにして一旦出る
				if (!pCommandTemp->m_isInputButton)
				{
					pCommandTemp->m_isInputButton = true;
					pCommandTemp->m_pUIType->InputUIUVChange(button);
					break;
				}
				// コマンドの入力が行われていた場合次のポインタへ
				else
				{
					pCommandTemp = pCommandTemp->m_NextCommand;
				}
			}
			else
			{
				// 入力コマンドが同一でなく、現在候補のコマンドが入力済みだった場合次のコマンドにポインタをずらす
				if (pCommandTemp->m_isInputButton)
				{
					pCommandTemp = pCommandTemp->m_NextCommand;
				}
				else
				{
					break;
				}
			}
		}
	}
}

// EOF