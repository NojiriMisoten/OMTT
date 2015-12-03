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
#include "CCommandChartManager.h"
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
static const float COMMAND_NAME_ADD_NUM = 7.0f;	// コマンドネームの変化の値の数
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//フェードアウト時の目標座標自分のID１
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//フェードアウト時の目標座標自分のID２
static const float BACK_POLYGON_X_SIZE = (COMMAND_POLYGON_WIDTH*MAX_COMMAND_KEEP) + (UI_X_POS_ADD*MAX_COMMAND_KEEP);	// コマンドチャートUIの背後に表示するポリゴンのXのサイズ
static const float BACK_POLYGON_Y_SIZE = 150.0f;	// コマンドチャートUIの背後に表示するポリゴンのYのサイズ
static const int COMMAND_DEATH_COUNT = 60;		// コマンド消去までのカウント
static const int COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM = 0;	// 弱攻撃のコマンドの配列番号
static const int COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM = 1;	// 中攻撃のコマンドの配列番号
static const int COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM = 2;	// 弱攻撃のコマンドの配列番号
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標ID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標
static const float ANIME_SPEED = 0.04f;			// 出たり消えたりのアニメーション速度
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
static const COMMAND_INFO COMMAND_ROLLING_ELBOW = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_ROLLING, COMMAND_BUTTON_ROLLING_ELBOW };

//フライングエルボー	RU RU LU RD
static const BUTTON_TYPE COMMAND_BUTTON_FLYING_ELBOW[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_3,
																			BUTTON_TYPE_3, 
																			BUTTON_TYPE_1,
																			BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_FLYING_ELBOW = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_SHOULDER, COMMAND_BUTTON_FLYING_ELBOW };

//ドロップキック		RU LU RU RU RD
static const BUTTON_TYPE COMMAND_BUTTON_DROP_KICK[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_3,
																		BUTTON_TYPE_1,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_4 };
static const COMMAND_INFO COMMAND_DROP_KICK = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_DROPKICK, COMMAND_BUTTON_DROP_KICK };

//ビンタ			LD LU RU
static const BUTTON_TYPE COMMAND_BUTTON_FACE_SLAPPING[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_4,
																			BUTTON_TYPE_3,
																			BUTTON_TYPE_1 };
static const COMMAND_INFO COMMAND_FACE_SLAPPING = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_SLAP, COMMAND_BUTTON_FACE_SLAPPING };

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
static const COMMAND_INFO COMMAND_STANER = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_STUNNER, COMMAND_BUTTON_STANER };

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
static const COMMAND_INFO* COMMAND_SMALL_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_CHOP,
																			  &COMMAND_ROLLING_ELBOW,
																			  &COMMAND_FACE_SLAPPING};
// 中技
// エルボー
// フライングエルボー
// バックドロップ
static const COMMAND_INFO* COMMAND_MIDDLE_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_ELBOW,
																			  &COMMAND_FLYING_ELBOW,
																			  &COMMAND_BACKDROP };

// 大技
// ラリアット
// ドロップキック
// スタナー
static const COMMAND_INFO* COMMAND_LARGE_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_LARIAT,
																			  &COMMAND_DROP_KICK,
																			  &COMMAND_STANER };

// 左上キー開始コマンドをまとめた配列
// チョップ
// エルボー
// ラリアット
static const COMMAND_INFO* COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MAX] = {&COMMAND_CHOP,
																				&COMMAND_ELBOW,
																				&COMMAND_LARIAT};

// 右上キー開始コマンドをまとめた配列
// ローリングエルボー
// フライングエルボー
// ドロップキック
static const COMMAND_INFO* COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_ROLLING_ELBOW,
																				&COMMAND_FLYING_ELBOW,
																				&COMMAND_DROP_KICK};

// 左下キー開始コマンドをまとめた配列
// ビンタ
// バックドロップ
// スタナー
static const COMMAND_INFO* COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_FACE_SLAPPING,
																					&COMMAND_BACKDROP,
																					&COMMAND_STANER };

// 右下キー開始コマンドをまとめた配列
// ラリアット
// スタナー
// ドロップキック
static const COMMAND_INFO* COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_LARIAT,
																					&COMMAND_STANER,
																					&COMMAND_DROP_KICK };

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
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID, CCommandChartManager* pCommandManager)
{
	// デバイスのポインタを保存
	m_pD3DDevice = pDevice;

	// 入力されたコマンド保持用変数の初期化
	m_aCommandKeep = BUTTON_TYPE_NONE;

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
	m_pBackPolygon->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBackPolygon->SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f));

	m_CommandChartMode = MODE_INPUT;

	m_CompleteCommand = COMMAND_TYPE_NONE;

	m_pCommandManager = pCommandManager;
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
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, int nID, CCommandChartManager* pCommandManager)
{
	// コマンドチャートの生成
	CCommandChart* temp = new CCommandChart(pDevice, nID, pCommandManager);
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

	//*******************追記開始11/23　野尻 **************************************
	// 最初のコマンド作成 表示はしない
	InitCreateBeginCommand();

	// 最初以外のコマンド作成 表示はしない
	InitCreateCommandList();

	// 最初のコマンドのみ表示
	ResetAllCommand();
	//*******************追記終了11/23　野尻***************************************

	// 開いたり閉じたりアニメの初期化処理のまとめ
	InitAnime();
}

//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
#ifdef _DEBUG
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_COMMAND_ANIME_CLOSE))
	{
		m_CommandChartMode = MODE_VANISH;
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_COMMAND_ANIME_OPEN))
	{
		m_CommandChartMode = MODE_APPEAR;
	}
#endif
	// コマンドリセット とりあえず、たぶんあとで買える　CCommandChart::MODE_RESET
	m_CompleteCommand = COMMAND_TYPE_NONE;

	// アニメーション更新まとめ
	UpdateAnime();

	switch (m_CommandChartMode)
	{
	case MODE_APPEAR:
		// コマンドチャート出現
		AppearanceCommandChart();
		break;

	case MODE_INPUT:
		// コマンド入力可能
		if (m_isCommandInput)
		{
			// コマンド入力
			InputCommand();

			// コマンドのチェック
			CheckCommand();
			m_aCommandKeep = BUTTON_TYPE_NONE;
		}
		break;

	case MODE_COMPLETE_COMMAND:
		// 技入力完成の演出　いい感じで

		// ジャッジにセットしたい

		// TODO
		// すぐにリセットしないで待機時間を設ける
		//m_CommandChartMode = MODE_RESET;
		break;

	case MODE_VANISH:
		// コマンドチャート消える
		VanishCommandChart();
		break;

	case MODE_RESET:
		// リセット
		ResetCommandList();
		m_CommandChartMode = MODE_INPUT;
		break;
	
	case MODE_ROPE:
		SetRopeCommand();
		m_CommandChartMode = MODE_INPUT;
		break;

	default:
		break;
	}
#ifdef _DEBUG
	CDebugProc::PrintU("技タイプ:%d\n", m_CompleteCommand);
#endif
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

	}
	// ボタンタイプ２
	else if (isPushButton2)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_2;

	}
	// ボタンタイプ３
	else if (isPushButton3)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_3;

	}
	// ボタンタイプ４
	else if (isPushButton4)
	{
		// キー入力の保存
		m_aCommandKeep = BUTTON_TYPE_4;

	}
	//*******************変更開始11/23　野尻 **************************************
	// 何も押されてないなら終了
	else
	{
		return;
	}

	// コマンド保持数の増加
	m_nKeepCommandNum++;

	// 描画するx座標の更新
	m_fPosX += UI_X_POS_ADD;

	// 最初のコマンド入力の時の処理
	if (m_nKeepCommandNum == 1)
	{
		//*******************変更開始11/23　野尻 **************************************
		// 次に入力すべきコマンドのリセット
		ResetNextCommand();
		//*******************変更終了11/23　野尻 **************************************

		// 発生候補の技名表示用UIの目標座標の設定
		// 技名表示用UIの初期座標の設定
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
	}
	// 2回目からのコマンドの入力の時の処理
	else
	{
		// コマンドの入力チェック
		CommandUIInput(m_aCommandKeep);
	}

	// コマンド入力からコマンド初期化までの間の初期化
	m_nCommandDeathCnt = 0;
	//*******************変更終了11/23　野尻 **************************************
}


//-----------------------------------------------------------------------------
// コマンドのチェック
//-----------------------------------------------------------------------------
void CCommandChart::CheckCommand(void)
{
	// フィニッシュの判定はまだなし

	SKILL_TYPE type = SKILL_MAX;


	//*******************変更開始11/23　野尻 **************************************
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 終端が押されてるかを調べる
		if (!m_CommandInfo.commandList.smallAttack[j].isEndList)
		{
			continue;
		}
		if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetInputFlag())
		{
			type = SKILL_SMALL_ATTACK;
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 終端が押されてるかを調べる
		if (!m_CommandInfo.commandList.middleAttack[j].isEndList)
		{
			continue;
		}
		if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetInputFlag())
		{
			type = SKILL_MIDDLE_ATTACK;
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 終端が押されてるかを調べる
		if (!m_CommandInfo.commandList.largeAttack[j].isEndList)
		{
			continue;
		}
		if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetInputFlag())
		{
			type = SKILL_BIG_ATTACK;
		}
		break;
	}

	// 何も完成していないなら返す
	if (type == SKILL_MAX)
	{
		return;
	}

	// 押されてる始動技から種類判別
	int idx = -1;
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (!m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->GetInputFlag())
		{
			continue;
		}

		BUTTON_TYPE buttonType = m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->GetButtonType();
		if (buttonType == BUTTON_TYPE_3)
		{
			idx = 0;
		}
		else if (buttonType == BUTTON_TYPE_1)
		{
			idx = 1;
		}
		else
		{
			idx = 2;
		}
		break;
	}
	if (idx == -1)
	{
		return;
	}

	// コマンド判別
	switch (type)
	{
	case SKILL_SMALL_ATTACK:
		m_CompleteCommand = COMMAND_SMALL_TECHNIQUE_LIST[idx]->m_CommandType;
		break;

	case SKILL_MIDDLE_ATTACK:
		m_CompleteCommand = COMMAND_MIDDLE_TECHNIQUE_LIST[idx]->m_CommandType;
		break;

	case SKILL_BIG_ATTACK:
		m_CompleteCommand = COMMAND_LARGE_TECHNIQUE_LIST[idx]->m_CommandType;
		break;

	default:
		return;
		break;
	}

	// 状態を完成状態へ
	m_CommandChartMode = MODE_COMPLETE_COMMAND;
	//*******************変更終了11/23　野尻 **************************************
}

//-----------------------------------------------------------------------------
// 技の確定
// 戻り値：繰り出す技
//
// コマンドが長いものの方が優先される
//-----------------------------------------------------------------------------
COMMAND_TYPE CCommandChart::UseTechnic(void)
{
	// 決定した技を返す
	return m_CompleteCommand;
}

//-----------------------------------------------------------------------------
// 右上キー開始コマンドの生成
// 引数：何個目のコマンドか
//-----------------------------------------------------------------------------
void CCommandChart::CreateRightUpTechnicCommand(void)
{
	//*******************変更開始11/23　野尻 **************************************
	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);
		
		// 描画ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************変更終了11/23　野尻 **************************************
}

//-----------------------------------------------------------------------------
// 左上キー開始のコマンドの生成
// 引数：何個目のコマンドか
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftUpTechnicCommand(void)
{
	//*******************変更開始11/23　野尻 **************************************
	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************変更終了11/23　野尻 **************************************
}

//-----------------------------------------------------------------------------
// 左下キー開始コマンドの生成
// 引数：何個目のコマンドか
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftDownTechnicCommand(void)
{
	//*******************変更開始11/23　野尻 **************************************
	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************変更終了11/23　野尻 **************************************
}

//-----------------------------------------------------------------------------
// 右下キー開始のコマンドチャートの生成
//-----------------------------------------------------------------------------
void CCommandChart::CreateRightDownTechnicCommand(void)
{
	//*******************変更開始11/23　野尻 **************************************
	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// 描画ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************変更終了11/23　野尻 **************************************
}
//-----------------------------------------------------------------------------
//	コマンドUIの入力
//-----------------------------------------------------------------------------
void CCommandChart::CommandUIInput(BUTTON_TYPE button)
{
	// コマンドの弱中強のリストを調べる
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// ボタン比較
		if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetButtonType() == button)
		{
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->InputUIUVChange(button, true);
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// ボタン比較
		if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetButtonType() == button)
		{
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->InputUIUVChange(button, true);
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// ボタン比較
		if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetButtonType() == button)
		{
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->InputUIUVChange(button, true);
		}
		break;
	}
}


//*******************追記開始11/23　野尻 **************************************
//-----------------------------------------------------------------------------
// 始動コマンドの作成 
//-----------------------------------------------------------------------------
void CCommandChart::InitCreateBeginCommand(void)
{
	// 次に表示するコマンドの生成座標
	float fPosX = 0.0f;
	
	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyID == MY_ID_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
																						, type
																						, pos
																						,TEXTURE_BUTTON);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDestPos(pos);

		// アニメーション用に座標を保存
		m_CommandInfo.beginCommand.firstCommand[i].vAnimationPosDest = pos;

		pos.y += NEXT_UI_Y_POS_ADD;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
	}
}

//-----------------------------------------------------------------------------
// 次に入力する候補のコマンドの作成
//-----------------------------------------------------------------------------
void CCommandChart::InitCreateCommandList(void)
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
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	fPosY = UI_Y_POSITION;

	// 小技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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
	
		m_CommandInfo.commandList.smallAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
																					, BUTTON_TYPE_1
																					, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// 生成位置
																					, TEXTURE_BUTTON);
		// 生成後目指す座標の設定
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// アニメーション用に座標を保存
		m_CommandInfo.commandList.smallAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// 表示はしない
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);

		// 終端フラグの設定
		m_CommandInfo.commandList.smallAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_SMALL - 2)
		{
			m_CommandInfo.commandList.smallAttack[j].isEndList = true;
		}
	}

	// コマンドを表記するためのY座標の設定
	fPosY += NEXT_UI_Y_POS_ADD;

	// 中技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		m_CommandInfo.commandList.middleAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
			, BUTTON_TYPE_1
			, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// 生成位置
			, TEXTURE_BUTTON);
		// 生成後目指す座標の設定
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// アニメーション用に座標を保存
		m_CommandInfo.commandList.middleAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// 表示はしない
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);

		// 終端フラグの設定
		m_CommandInfo.commandList.middleAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_MIDDLE - 2)
		{
			m_CommandInfo.commandList.middleAttack[j].isEndList = true;
		}
	}

	// コマンドを表記するためのY座標の設定
	fPosY += NEXT_UI_Y_POS_ADD;

	// 大技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		m_CommandInfo.commandList.largeAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
			, BUTTON_TYPE_1
			, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// 生成位置
			, TEXTURE_BUTTON);
		// 生成後目指す座標の設定
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// アニメーション用に座標を保存
		m_CommandInfo.commandList.largeAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// 表示はしない
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);

		// 終端フラグの設定
		m_CommandInfo.commandList.largeAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_LARGE - 2)
		{
			m_CommandInfo.commandList.largeAttack[j].isEndList = true;
		}
	}
}

//-----------------------------------------------------------------------------
// 次に入力する候補のコマンドのリセット
//-----------------------------------------------------------------------------
void CCommandChart::ResetNextCommand(void)
{
	switch (m_aCommandKeep)
	{
		// Qもしくは右側の上ボタンに対応
	case BUTTON_TYPE_1:
		CreateRightUpTechnicCommand();
		break;
		// Wもしくは右側の下ボタンに対応
	case BUTTON_TYPE_2:
		m_pCommandManager->SetCommandChartMode(MY_ID_1, MODE_ROPE);
		m_pCommandManager->SetCommandChartMode(MY_ID_2, MODE_ROPE);
		break;
		// Aもしくは左側の上ボタンに対応
	case BUTTON_TYPE_3:
		CreateLeftUpTechnicCommand();
		break;
		// Sもしくは左側の下ボタンに対応
	case BUTTON_TYPE_4:
		CreateLeftDownTechnicCommand();
		break;
	default:
		break;
	}
	
	// 先頭コマンドの処理
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (i == m_aCommandKeep - 1)
		{
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
			continue;
		}
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
	}

	//--------
	// m_apCommandNameの座標と技名テクスチャ変える

	//--------
}

//-----------------------------------------------------------------------------
// 始動コマンドだけの状態になる
//-----------------------------------------------------------------------------
void CCommandChart::ResetCommandList(void)
{
	// コマンド保持数の増加
	m_nKeepCommandNum = 0;

	// 描画するx座標の更新
	m_fPosX = 0;

	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
	}

	// コマンドリストの初期化
	// 小技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetInputFlag(false);
	}

	// 中技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetInputFlag(false);
	}

	// 大技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetInputFlag(false);
	}

	// 技名表示ポリゴンを初期座標へ
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

//-----------------------------------------------------------------------------
// 位置やテクスチャなどの何回も呼べる初期化
//-----------------------------------------------------------------------------
void CCommandChart::ResetAllCommand(void)
{

	// コマンド保持数の増加
	m_nKeepCommandNum = 0;

	// 描画するx座標の更新
	m_fPosX = 0;

	// 始動コマンドの初期化
	float fPosX = 0.0f;
	float fPosDestX = 0.0f;
	float fPosY = 0.f;

	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyID == MY_ID_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == MY_ID_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
// これなに　やめて
//		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDestPos(pos);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
		pos.y += NEXT_UI_Y_POS_ADD;
	}
	
	// コマンドリストの初期化

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

	fPosY = UI_Y_POSITION;

	// 小技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		// 生成後目指す座標の設定
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// 表示はしない
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetInputFlag(false);
	}

	// コマンドを表記するためのY座標の設定
	fPosY += NEXT_UI_Y_POS_ADD;

	// 中技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		// 生成後目指す座標の設定
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// 表示はしない
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetInputFlag(false);
	}

	// コマンドを表記するためのY座標の設定
	fPosY += NEXT_UI_Y_POS_ADD;

	// 大技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		// 生成後目指す座標の設定
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// 表示はしない
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetInputFlag(false);
	}

	// 技名表示ポリゴンを初期座標へ
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

//-----------------------------------------------------------------------------
// コマンドチャート表示
//-----------------------------------------------------------------------------
void CCommandChart::AppearanceCommandChart(void)
{
	// 技名表示用UIの初期座標の設定
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		// 発生候補の技名表示用UIを描画オン
		m_apCommandName[i]->SetDrawFlag(true);

		//--------
		// m_apCommandNameの座標と技名テクスチャ変える

		//--------
	}

	// アニメーションの開始
	StartAnimeOpen();
}

//-----------------------------------------------------------------------------
// コマンドチャート消す
//-----------------------------------------------------------------------------
void CCommandChart::VanishCommandChart(void)
{
	// 閉じるアニメーション開始
	StartAnimeClose();
}

//-----------------------------------------------------------------------------
// コマンド消す
//-----------------------------------------------------------------------------
void CCommandChart::VanishCommand(void)
{
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
	}

	// コマンドリストの初期化
	// 小技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);
	}

	// 中技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);
	}

	// 大技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);
	}
}
//*******************追記終了11/23　野尻 **************************************


//-----------------------------------------------------------------------------
// アニメーション用の初期化関数 このクラスのInitで呼ぶ
//-----------------------------------------------------------------------------
void CCommandChart::InitAnime()
{
	m_isAnime = false;
	m_isAnimeOpen = false;
	m_AnimeCount = 0;
}

//-----------------------------------------------------------------------------
// 更新で呼ぶ。アニメーションの更新まとめ
//-----------------------------------------------------------------------------
void CCommandChart::UpdateAnime()
{
	// アニメーションするよー
	if (!m_isAnime)	return;

	// アニメーション速度
	m_AnimeCount += ANIME_SPEED;

	// アニメ終わり
	if (m_AnimeCount > 1.0f)
	{
		// アニメフラグOFF
		m_isAnime = false;
		// コマンドチャートの更新モードを変更
		if (m_isAnimeOpen)	m_CommandChartMode = MODE_INPUT;
		else				m_CommandChartMode = MODE_MAX;
	}
	else
	{
		float width, height;
		D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
		// 背景のポリゴン
		width = m_Back.GetEasingWidth(m_AnimeCount);
		height = m_Back.GetEasingHeight(m_AnimeCount);
		m_pBackPolygon->SetVertexPolygon(m_Back.m_Pos, width, height);

		// 小攻撃
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			width = m_CommandSmall[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandSmall[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandSmall[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandSmall[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// 中攻撃
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			width = m_CommandMiddle[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandMiddle[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandMiddle[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandMiddle[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// 大攻撃
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			width = m_CommandLarge[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandLarge[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandLarge[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandLarge[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// 最初のコマンド
		for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
		{
			width = m_CommandFirst[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandFirst[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandFirst[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandFirst[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.beginCommand.firstCommand[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// 技名
		for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
		{
			width = m_CommandName[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandName[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandName[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandName[j].GetEasingPosY(m_AnimeCount);
			m_apCommandName[j]->SetVertexPolygon(pos, width, height);
		}
	}
}

//-----------------------------------------------------------------------------
//	開くアニメーション開始
// 各ポリゴンのアニメーション用の値を決める
//-----------------------------------------------------------------------------
void CCommandChart::StartAnimeOpen(void)
{
	// アニメーションしているときは何もしない
	if (m_isAnime)
	{
		return;
	}

	// なんか初期化
	ResetAllCommand();

	// アニメーションに必要な変数初期化
	m_isAnime = true;
	m_isAnimeOpen = true;
	m_AnimeCount = 0;
	// 一時変数
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 posDest = D3DXVECTOR3(0, 0, 0);

	// 出現位置は共通で背景ポリゴンの座標
	pos = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	// 背景のポリゴン
	posDest = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	m_Back.Init(pos, posDest, 0, BACK_POLYGON_X_SIZE, 0, BACK_POLYGON_Y_SIZE);

	// 小技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.smallAttack[j].vAnimationPosDest;
		m_CommandSmall[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// 中技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.middleAttack[j].vAnimationPosDest;
		m_CommandMiddle[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// 大技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.largeAttack[j].vAnimationPosDest;
		m_CommandLarge[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// 最初のコマンド
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		posDest = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// 技名
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		if (m_MyID == MY_ID_1)
		{
			posDest = D3DXVECTOR3(UI_X_POSITION + UI_X_POS_ADD + UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
		}
		else if (m_MyID == MY_ID_2)
		{
			posDest = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
		}
		m_CommandName[j].Init(pos, posDest, 0, COMMAND_NAME_POLYGON_WIDTH, 0, COMMAND_NAME_POLYGON_HEIGHT);
	}
}

//-----------------------------------------------------------------------------
//	閉じるアニメーション開始
// 各ポリゴンのアニメーション用の値を決める
//-----------------------------------------------------------------------------
void CCommandChart::StartAnimeClose(void)
{
	// アニメーションしているときは何もしない
	if (m_isAnime)	return;

	// アニメーションに必要な変数初期化
	m_isAnime = true;
	m_isAnimeOpen = false;
	m_AnimeCount = 0;
	// 一時変数
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 posDest = D3DXVECTOR3(0, 0, 0);

	// すべて背景ポリゴンの座標に消える
	posDest = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;

	// 背景のポリゴン
	pos = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	m_Back.Init(pos, posDest, BACK_POLYGON_X_SIZE, 0, BACK_POLYGON_Y_SIZE, 0);
	// 小技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.smallAttack[j].vAnimationPosDest;
		m_CommandSmall[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// 中技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.middleAttack[j].vAnimationPosDest;
		m_CommandMiddle[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// 大技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.largeAttack[j].vAnimationPosDest;
		m_CommandLarge[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}

	// 最初のコマンド
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		pos = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// 技名
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		pos = m_apCommandName[j]->GetPos();
		m_CommandName[j].Init(pos, posDest, COMMAND_NAME_POLYGON_WIDTH, 0, COMMAND_NAME_POLYGON_HEIGHT, 0);
	}
}

//-----------------------------------------------------------------------------
// ロープコマンドをセット
//-----------------------------------------------------------------------------
void CCommandChart::SetRopeCommand(void)
{
	// リセット
	ResetAllCommand();
	m_aCommandKeep = BUTTON_TYPE_2;

	// コマンド保持数の増加
	m_nKeepCommandNum++;

	// 描画するx座標の更新
	m_fPosX += UI_X_POS_ADD;

	// 最初のコマンド入力の時の処理
	if (m_nKeepCommandNum == 1)
	{
		// 発生候補の技名表示用UIの目標座標の設定
		// 技名表示用UIの初期座標の設定
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
		CreateRightDownTechnicCommand();

		// 先頭コマンドの処理
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (i == m_aCommandKeep - 1)
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
				continue;
			}
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
		}
	}
	m_aCommandKeep = BUTTON_TYPE_NONE;
}

// EOF