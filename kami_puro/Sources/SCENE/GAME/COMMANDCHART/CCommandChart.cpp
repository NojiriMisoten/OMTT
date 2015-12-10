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
static const float UI_Y_POSITION = 350.0f;		// UIの初期表示座標(y座標)
static const float UI_X_POS_ADD = 30.0f;		// １つ表示後のUIの座標の変化の値
static const float NEXT_UI_X_POS = 50.0f;		// 次に入力するコマンドの一番上のUIの座標
static const float NEXT_UI_Y_POS = 350.0f;		// 次に入力するコマンドの一番上のUIの座標
static const float NEXT_UI_X_POS_ADD = COMMAND_POLYGON_WIDTH * 1.0f;	// 次に入力するコマンドのUIの座標の変化の値
static const float NEXT_UI_Y_POS_ADD = 33.0f * (SCREEN_HEIGHT * 0.5f / 150.f);	// 次に入力するコマンドのUIの座標の変化の値
static const float COMMAND_NAME_ADD_NUM = 7.0f;	// コマンドネームの変化の値の数
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//フェードアウト時の目標座標自分のID１
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//フェードアウト時の目標座標自分のID２
static const float BACK_POLYGON_X_SIZE = ((COMMAND_POLYGON_WIDTH * MAX_COMMAND_KEEP) + (UI_X_POS_ADD * MAX_COMMAND_KEEP));	// コマンドチャートUIの背後に表示するポリゴンのXのサイズ
static const float BACK_POLYGON_Y_SIZE = SCREEN_HEIGHT * 0.6f;	// コマンドチャートUIの背後に表示するポリゴンのYのサイズ
static const int COMMAND_DEATH_COUNT = 60;		// コマンド消去までのカウント
static const int COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM = 0;	// 弱攻撃のコマンドの配列番号
static const int COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM = 1;	// 中攻撃のコマンドの配列番号
static const int COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM = 2;	// 弱攻撃のコマンドの配列番号
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION * 1.3f - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (40.0f * (SCREEN_HEIGHT * 0.5f / 150.f)*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標ID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION * 1.3f - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (40.0f * (SCREEN_HEIGHT * 0.5f / 150.f)*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// 背後のポリゴンの座標
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

// 始動コマンドのテクスチャの配列
static const TEXTURE_TYPE BEGIN_COMMAND_TEXTURE_TYPE[CCommandChart::MAX_BEGIN_COMAND_NUM]
{
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
};

// 左上始動コマンドのテクスチャの配列
static const TEXTURE_TYPE LEFT_UP_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_CHOP,
	TEXTURE_SKILL_NAME_ELBOW,
	TEXTURE_SKILL_NAME_LARIAT,
};
// 右上始動コマンドのテクスチャの配列
static const TEXTURE_TYPE RIGHT_UP_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_ROLLING,
	TEXTURE_SKILL_NAME_SHOULDER,
	TEXTURE_SKILL_NAME_DROPKICK,
};

// 左下始動コマンドのテクスチャの配列
static const TEXTURE_TYPE LEFT_DOWN_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_SLAP,
	TEXTURE_SKILL_NAME_BACKDROP,
	TEXTURE_SKILL_NAME_STUNNER,
};

// 右下始動コマンドのテクスチャの配列
static const TEXTURE_TYPE RIGHT_DOWN_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_LARIAT,
	TEXTURE_SKILL_NAME_STUNNER,
	TEXTURE_SKILL_NAME_DROPKICK,
};

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager)
{
	// デバイスのポインタを保存
	m_pD3DDevice = pDevice;

	// 入力されたコマンド保持用変数の初期化
	m_aCommandKeep = BUTTON_TYPE_NONE;

	// 自身のプレイヤー番号を設定
	m_MyID = nID;
	
	// 背後に表示するポリゴンの作成
	m_pBackPolygon = new CScene2D(pDevice);
	if (m_MyID == PLAYER_1)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_1, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_COMMAND_CHART_BACK);
	}
	else if (m_MyID == PLAYER_2)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_2, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_COMMAND_CHART_BACK);
	}
	m_pBackPolygon->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBackPolygon->SetDrawFlag(false);

	m_CommandChartMode = MODE_MAX;

	m_DestCompleteCommand = m_CompleteCommand = COMMAND_TYPE_NONE;

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
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager)
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
	for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
	{
		if (m_MyID == PLAYER_1)
		{
			pos = D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}
		else if (m_MyID == PLAYER_2)
		{
			pos = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}

		// 発生候補の技名表示用UIを作成
		m_apCommandName[i] = CCommandName::Create(m_pD3DDevice, pos, BEGIN_COMMAND_TEXTURE_TYPE[i]);
		m_apCommandName[i]->SetDrawFlag(false);
	}

	// 最初のコマンド作成 表示はしない
	InitCreateBeginCommand();

	// 最初以外のコマンド作成 表示はしない
	InitCreateCommandList();

	// 開いたり閉じたりアニメの初期化処理のまとめ
	InitAnime();

	for (int i = 0; i < MAX_KEEP_COMMAND_NUM; i++)
	{
		m_KeepButton[i].buttonType = BUTTON_TYPE_NONE;
		m_KeepButton[i].isUse = false;
	}
	m_WiatCompleteCommandTimer = 0;
	m_CompleteSkill = SKILL_MAX;
}

//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
#ifdef _DEBUG
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_COMMAND_ANIME_CLOSE))
	{
		m_CommandChartMode = MODE_RESET;
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

			// FINISH技始動ボタン表示(中で表示するかの判定してる)
			isAppearFinishBeginCommand();

			// FINISH技作る
			bool isCreateFinishCommand = (!m_CommandInfo.beginCommand.firstCommand[4].pCommandUI->GetInputFlag())
											&& (GetSameTimePushButton(BUTTON_TYPE_5)
											&& m_pCommandManager->GetCanUseFinishSkill(m_MyID));
			if (isCreateFinishCommand)
			{
				CreateFinishTechnicCommand();
			}
			else
			{
				// コマンドのチェック
				CheckCommand();
			}
		}
		break;

	case MODE_PRE_COMPLETE_COMMAND:
		if (m_WiatCompleteCommandTimer < 0)
		{
			return;
		}
		if (m_WiatCompleteCommandTimer >= MAX_KEEP_COMMAND_NUM)
		{
			m_CommandChartMode = MODE_COMPLETE_COMMAND;
			break;
		}
		else
		{
			// 入力確認
			CheckPushCommand();

			// FINISH完成確認
			CheckPushForFinishCommand();

			if (isCompleteFinishSkill())
			{
				m_DestCompleteCommand = COMMAND_TYPE_FINISHER;
			}
		}
		m_WiatCompleteCommandTimer++;
		break;

	case MODE_COMPLETE_COMMAND:

		if (m_WiatCompleteCommandTimer >= MAX_KEEP_COMMAND_NUM)
		{
			VanishOtherSkill(m_CompleteSkill);
			m_CompleteCommand = m_DestCompleteCommand;
			m_DestCompleteCommand = COMMAND_TYPE_NONE;
			m_CompleteSkill = SKILL_MAX;
			m_WiatCompleteCommandTimer = -1;
			// コマンドチャートの後ろのポリゴンを消す
			AllCandidateInputBackPolygonVanish();
		}

		break;
		
	case MODE_VANISH:
		// コマンドチャート消える
		VanishCommandChart();
		m_CompleteCommand = COMMAND_TYPE_NONE;
		m_CommandChartMode = MODE_MAX;
		break;

	case MODE_RESET:
		// リセット
		ResetCommandList();
		m_CommandChartMode = MODE_INPUT;
		m_WiatCompleteCommandTimer = 0;
		m_CompleteCommand = COMMAND_TYPE_NONE;
		break;
	
	case MODE_ROPE:
		SetRopeCommand();
		m_CommandChartMode = MODE_INPUT;
		break;

	default:
		break;
	}

	m_aCommandKeep = BUTTON_TYPE_NONE;
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
	CheckPushCommand();

	// 何も押されてないなら終了
	if (m_aCommandKeep == BUTTON_TYPE_NONE)
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
		for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
		{
			if (m_MyID == PLAYER_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION * 4.5f, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == PLAYER_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION * 4.f - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
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
	SKILL_TYPE type = SKILL_MAX;
	bool isRope = false;

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

	// FINISH技の判定
	if (isCompleteFinishSkill())
	{
		type = SKILL_FINISH_ATTACK;
	}

	// 何も完成していないなら返す
	if (type == SKILL_MAX)
	{
		return;
	}

	// FINISH技最優先
	if (type == SKILL_FINISH_ATTACK)
	{
		// 状態を完成状態へ
		m_CommandChartMode = MODE_PRE_COMPLETE_COMMAND;
		m_DestCompleteCommand = COMMAND_TYPE_FINISHER;
		m_CompleteSkill = type;
		return;
	}

	// 押されてる始動技から種類判別
	int idx = -1;
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
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
		else if (buttonType == BUTTON_TYPE_4)
		{
			idx = 2;
		}
		else if (buttonType == BUTTON_TYPE_2)
		{
			idx = 0;
			isRope = true;
		}
		break;
	}
	if (idx == -1)
	{
		return;
	}

	if (!isRope)
	{
		// コマンド判別
		switch (type)
		{
		case SKILL_SMALL_ATTACK:
			m_DestCompleteCommand = COMMAND_SMALL_TECHNIQUE_LIST[idx]->m_CommandType;
			break;

		case SKILL_MIDDLE_ATTACK:
			m_DestCompleteCommand = COMMAND_MIDDLE_TECHNIQUE_LIST[idx]->m_CommandType;
			break;

		case SKILL_BIG_ATTACK:
			m_DestCompleteCommand = COMMAND_LARGE_TECHNIQUE_LIST[idx]->m_CommandType;
			break;

		default:
			return;
			break;
		}
	}
	else
	{
		if (type < SKILL_MAX)
		{
			m_DestCompleteCommand = COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[type]->m_CommandType;
		}
	}
	m_CompleteSkill = type;

	// 状態を完成状態へ
	m_CommandChartMode = MODE_PRE_COMPLETE_COMMAND;
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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
			m_CommandInfo.commandList.largeAttack[j].isEndList = true;
		}

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// 技名変更
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(RIGHT_UP_COMMAND_TEXTURE_TYPE[j]);
	}

	// 4つめはないので名前消す
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
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
		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// 技名変更
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(LEFT_UP_COMMAND_TEXTURE_TYPE[j]);
	}
	// 4つめはないので名前消す
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// 技名変更
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(LEFT_DOWN_COMMAND_TEXTURE_TYPE[j]);
	}

	// ４つめはないので名前消す
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// 技名変更
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(RIGHT_DOWN_COMMAND_TEXTURE_TYPE[j]);
	}

	// ４つめはないので名前消す
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
	//*******************変更終了11/23　野尻 **************************************
}

//-----------------------------------------------------------------------------
// FINISHコマンドチャートの生成
//-----------------------------------------------------------------------------
void CCommandChart::CreateFinishTechnicCommand(void)
{
	// 先頭は除くので１から始める
	for (int j = 1; j < COMMAND_FINISHER.m_nCommandLength; j++)
	{
		// UV値設定
		m_CommandInfo.commandList.finishAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_BUTTON_FINISHER[j], false);

		// 描画ON
		m_CommandInfo.commandList.finishAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.finishAttack[j - 1].isEndList = false;
		if (j == COMMAND_FINISHER.m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.finishAttack[j - 1].isEndList = true;
		}

		// コマンドの背景のポリゴン表示
		if (j == 1)
		{
			m_CommandInfo.commandList.finishAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(true);
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->InputUIUVChange(COMMAND_BUTTON_FINISHER[0], true);
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

			// リストの最後のコマンドで無ければ次のコマンドにポリゴンを表示する
			if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.smallAttack[j].isEndList)

			{
				m_CommandInfo.commandList.smallAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.smallAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

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

			// リストの最後のコマンドで無ければ次のコマンドにポリゴンを表示する
			if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.middleAttack[j].isEndList)

			{
				m_CommandInfo.commandList.middleAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.middleAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

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

			// リストの最後のコマンドで無ければ次のコマンドにポリゴンを表示する
			if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.largeAttack[j].isEndList)
			{
				m_CommandInfo.commandList.largeAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.largeAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

		}
		break;
	}

	// FINISH技のコマンド押されてるか
	CheckPushForFinishCommand();
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
	if (m_MyID == PLAYER_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == PLAYER_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
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
	if (m_MyID == PLAYER_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
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

	// コマンドを表記するためのY座標の設定
	fPosY += NEXT_UI_Y_POS_ADD * 2.f;

	// FINISH技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 目標の座標
		// プレイヤー１の時の表示X座標
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
		{
			fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
			fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
		}

		m_CommandInfo.commandList.finishAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
			, BUTTON_TYPE_1
			, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// 生成位置
			, TEXTURE_BUTTON);
		// 生成後目指す座標の設定
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// アニメーション用に座標を保存
		m_CommandInfo.commandList.finishAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// 表示はしない
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);

		// 終端フラグの設定
		m_CommandInfo.commandList.finishAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_FINISHER - 2)
		{
			m_CommandInfo.commandList.finishAttack[j].isEndList = true;
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
		// Wもしくは右側の上ボタンに対応
	case BUTTON_TYPE_1:
		CreateRightUpTechnicCommand();
		break;
		// Sもしくは右側の下ボタンに対応
	case BUTTON_TYPE_2:
		m_pCommandManager->SetCommandChartMode(PLAYER_1, MODE_ROPE);
		m_pCommandManager->SetCommandChartMode(PLAYER_2, MODE_ROPE);
		m_CompleteCommand = COMMAND_TYPE_ROPE;
		break;
		// Qもしくは左側の上ボタンに対応
	case BUTTON_TYPE_3:
		CreateLeftUpTechnicCommand();
		break;
		// Aもしくは左側の下ボタンに対応
	case BUTTON_TYPE_4:
		CreateLeftDownTechnicCommand();
		break;
	default:
		break;
	}
	
	// 先頭コマンドの処理 FINISH技だけは関係なし
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM - 1; i++)
	{
		if (i == m_aCommandKeep - 1)
		{
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
			D3DXVECTOR3 dest = m_CommandInfo.beginCommand.firstCommand[0].pCommandUI->GetPos();
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->PlaySelectedCommandMove(dest);
			continue;
		}
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
	}
}

//-----------------------------------------------------------------------------
// 始動コマンドだけの状態になる
//-----------------------------------------------------------------------------
void CCommandChart::ResetCommandList(void)
{
	m_WiatCompleteCommandTimer = 0;
	// コマンド保持数の増加
	m_nKeepCommandNum = 0;

	// 描画するx座標の更新
	m_fPosX = 0;

	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
	{
		if (i == MAX_BEGIN_COMAND_NUM - 1)
		{
			if (!m_pCommandManager->GetCanUseFinishSkill(m_MyID))
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
				continue;
			}
		}
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
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

	// FINISH技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetInputFlag(false);
	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);

	// 技名表示ポリゴンを初期座標へ
	for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
	{
		D3DXVECTOR3 pos;
		if (m_MyID == PLAYER_1)
		{
			pos = D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}
		else if (m_MyID == PLAYER_2)
		{
			pos = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}

		m_apCommandName[i]->SetDestPos(pos);
		m_apCommandName[i]->SetPos(pos);
		// 発生候補の技名表示用UIを描画オン
		m_apCommandName[i]->SetDrawFlag(true);
		m_apCommandName[i]->ChangeTexture(BEGIN_COMMAND_TEXTURE_TYPE[i]);
	}
	
	// 保持してたコマンド破棄
	RefleshKeepCommand();
}

//-----------------------------------------------------------------------------
// 位置やテクスチャなどの何回も呼べる初期化
//-----------------------------------------------------------------------------
void CCommandChart::ResetAllCommand(void)
{
	RefleshKeepCommand();
	m_WiatCompleteCommandTimer = 0;

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
	if (m_MyID == PLAYER_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == PLAYER_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}
	m_pBackPolygon->SetDrawFlag(true);

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
	{
		if (i == MAX_BEGIN_COMAND_NUM - 1)
		{
			if (!m_pCommandManager->GetCanUseFinishSkill(m_MyID))
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
				continue;
			}
		}
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetPos(pos);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDestPos(pos);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
		pos.y += NEXT_UI_Y_POS_ADD;
	}
	
	// コマンドリストの初期化

	// 目標の座標
	// プレイヤー１の時の表示X座標
	if (m_MyID == PLAYER_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// プレイヤー２の時の表示X座標
	else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
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

	// コマンドを表記するためのY座標の設定
	fPosY += NEXT_UI_Y_POS_ADD * 2.f;

	// FINISH技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 目標の座標
		// プレイヤー１の時の表示X座標
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// プレイヤー２の時の表示X座標
		else if (m_MyID == PLAYER_2)
		{
			fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
			fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
		}

		// 生成後目指す座標の設定
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// 表示はしない
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);

		// 入力初期化
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetInputFlag(false);
	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);

	// 技名表示ポリゴンを初期座標へ
	for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
	{
		if (m_MyID == PLAYER_1)
		{
			m_apCommandName[i]->SetDestPos(D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD+ (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
		}
		else if (m_MyID == PLAYER_2)
		{
			m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
		}
		m_apCommandName[i]->ChangeTexture(BEGIN_COMMAND_TEXTURE_TYPE[i]);
	}
}

//-----------------------------------------------------------------------------
// コマンドチャート表示
//-----------------------------------------------------------------------------
void CCommandChart::AppearanceCommandChart(void)
{
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
	RefleshKeepCommand();
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
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

	// FINISH技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);
	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);
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
	if (m_AnimeCount >= 1.0f)
	{
		m_AnimeCount = 1.0f;
	}
	
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
	// FINISH攻撃
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		width = m_CommandFinish[j].GetEasingWidth(m_AnimeCount);
		height = m_CommandFinish[j].GetEasingHeight(m_AnimeCount);
		pos.x = m_CommandFinish[j].GetEasingPosX(m_AnimeCount);
		pos.y = m_CommandFinish[j].GetEasingPosY(m_AnimeCount);
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
	}
	// 最初のコマンド
	for (int j = 0; j < MAX_BEGIN_COMAND_NUM; j++)
	{
		width = m_CommandFirst[j].GetEasingWidth(m_AnimeCount);
		height = m_CommandFirst[j].GetEasingHeight(m_AnimeCount);
		pos.x = m_CommandFirst[j].GetEasingPosX(m_AnimeCount);
		pos.y = m_CommandFirst[j].GetEasingPosY(m_AnimeCount);
		m_CommandInfo.beginCommand.firstCommand[j].pCommandUI->SetVertexPolygon(pos, width, height);
	}
	// 技名
	for (int j = 0; j < MAX_COMAND_NAME_NUM; j++)
	{
		width = m_CommandName[j].GetEasingWidth(m_AnimeCount);
		height = m_CommandName[j].GetEasingHeight(m_AnimeCount);
		pos.x = m_CommandName[j].GetEasingPosX(m_AnimeCount);
		pos.y = m_CommandName[j].GetEasingPosY(m_AnimeCount);
		m_apCommandName[j]->SetVertexPolygon(pos, width, height);
	}
	
	// アニメ終わり
	if (m_AnimeCount >= 1.0f)
	{
		// アニメフラグOFF
		m_isAnime = false;
		// コマンドチャートの更新モードを変更
		if (m_isAnimeOpen)	m_CommandChartMode = MODE_INPUT;
		else				m_CommandChartMode = MODE_MAX;
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

	// 技名表示用UIの初期座標の設定
	for (int i = 0; i < MAX_COMAND_NAME_NUM - 1; i++)
	{
		// 発生候補の技名表示用UIを描画オン
		m_apCommandName[i]->SetDrawFlag(true);
	}

	// アニメーションに必要な変数初期化
	m_isAnime = true;
	m_isAnimeOpen = true;
	m_AnimeCount = 0;
	// 一時変数
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 posDest = D3DXVECTOR3(0, 0, 0);

	// 出現位置は共通で背景ポリゴンの座標
	pos = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	// 背景のポリゴン
	posDest = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
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
	// FINISH技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.finishAttack[j].vAnimationPosDest;
		m_CommandFinish[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// 最初のコマンド
	for (int j = 0; j < MAX_BEGIN_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// 技名
	for (int j = 0; j < MAX_COMAND_NAME_NUM; j++)
	{
		if (m_MyID == PLAYER_1)
		{
			posDest = D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
		}
		else if (m_MyID == PLAYER_2)
		{
			posDest = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
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
	posDest = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;

	// 背景のポリゴン
	pos = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
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
	// FINISh技
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.finishAttack[j].vAnimationPosDest;
		m_CommandFinish[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}

	// 最初のコマンド
	for (int j = 0; j < MAX_BEGIN_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// 技名
	for (int j = 0; j < MAX_COMAND_NAME_NUM; j++)
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
		for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
		{
			if (m_MyID == PLAYER_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION * 4.5f, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == PLAYER_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION * 4.f - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
		CreateRightDownTechnicCommand();

		// 先頭コマンドの処理
		for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
		{
			if (i == m_aCommandKeep - 1)
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
				D3DXVECTOR3 dest = m_CommandInfo.beginCommand.firstCommand[0].pCommandUI->GetPos();
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->PlaySelectedCommandMove(dest);
				continue;
			}
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
		}
	}
	m_aCommandKeep = BUTTON_TYPE_NONE;
}

//-----------------------------------------------------------------------------
// 技完成時選択されてない技を消す
//-----------------------------------------------------------------------------
void CCommandChart::VanishOtherSkill(SKILL_TYPE completeSkill)
{
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (completeSkill != SKILL_BIG_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
		{
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);
		}
		if (completeSkill != SKILL_MIDDLE_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
		{
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);
		}
		if (completeSkill != SKILL_SMALL_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
		{
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);
		}
	}
	if (completeSkill != SKILL_BIG_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
	{
		m_apCommandName[SKILL_BIG_ATTACK]->SetDrawFlag(false);
	}
	if (completeSkill != SKILL_MIDDLE_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
	{
		m_apCommandName[SKILL_MIDDLE_ATTACK]->SetDrawFlag(false);
	}
	if (completeSkill != SKILL_SMALL_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
	{
		m_apCommandName[SKILL_SMALL_ATTACK]->SetDrawFlag(false);
	}
}

//-----------------------------------------------------------------------------
// 押されたボタン保持関数
//-----------------------------------------------------------------------------
void CCommandChart::KeepPushButton(bool isPushRightUp, bool isPushRightDown, bool isPushLeftUp, bool isPushLeftDown)
{
	int index = 0;

	// 全部使ってる時の処理
	if (m_KeepButton[MAX_KEEP_COMMAND_NUM - 1].isUse)
	{
		for (; index < MAX_KEEP_COMMAND_NUM; index++)
		{
			if (index >= MAX_KEEP_COMMAND_NUM - 1)
			{
				m_KeepButton[index].buttonType = BUTTON_TYPE_NONE;
				break;
			}
			m_KeepButton[index].buttonType = m_KeepButton[index + 1].buttonType;
		}
	}
	// 空きがあるなら
	else
	{
		for (; index < MAX_KEEP_COMMAND_NUM; index++)
		{
			if (!m_KeepButton[index].isUse)
			{
				break;
			}
		}
	}

	BUTTON_TYPE keepButton = BUTTON_TYPE_NONE;
	// 格納コマンド決定
	if (isPushRightUp && isPushLeftUp)
	{
		keepButton = BUTTON_TYPE_5;
	}
	else if (isPushRightDown && isPushLeftDown)
	{
		keepButton = BUTTON_TYPE_6;
	}
	else if (isPushRightUp)
	{
		keepButton = BUTTON_TYPE_1;
	}
	else if (isPushRightDown)
	{
		keepButton = BUTTON_TYPE_2;
	}
	else if (isPushLeftUp)
	{
		keepButton = BUTTON_TYPE_3;
	}
	else if (isPushLeftDown)
	{
		keepButton = BUTTON_TYPE_4;
	}
	m_KeepButton[index].buttonType = keepButton;
	m_KeepButton[index].isUse = true;
}

//=================================================================
// 同時押し判定関数
//=================================================================
bool CCommandChart::GetSameTimePushButton(BUTTON_TYPE type)
{
	if (type != BUTTON_TYPE_5 && type != BUTTON_TYPE_6)
	{
		return false;
	}

	bool isPushLeftUp = false;
	bool isPushLeftDown = false;
	bool isPushRightUp = false;
	bool isPushRightDown = false;
	bool isPushDoubleUp = false;
	bool isPushDoubleDown = false;

	for (int i = 0; i < MAX_KEEP_COMMAND_NUM; i++)
	{
		if (!m_KeepButton[i].isUse)
		{
			break;
		}
		if (m_KeepButton[i].buttonType == BUTTON_TYPE_1)
		{
			isPushRightUp = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_2)
		{
			isPushRightDown = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_3)
		{
			isPushLeftUp = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_4)
		{
			isPushLeftDown = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_5)
		{
			isPushDoubleUp = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_6)
		{
			isPushDoubleDown = true;
		}
	}

	// 判定したいの上同時押しなら
	if (type == BUTTON_TYPE_5)
	{
		if (isPushDoubleUp || (isPushLeftUp && isPushRightUp))
		{
			return true;
		}
	}

	else if (type == BUTTON_TYPE_6)
	{
		// 判定したいの下同時押しなら
		if (isPushDoubleDown || (isPushLeftDown && isPushRightDown))
		{
			return true;
		}
	}

	return false;
}

//=================================================================
// 保持してたコマンド破棄
//=================================================================
void CCommandChart::RefleshKeepCommand()
{
	// 保持してたやつ破棄
	for (int i = 0; i < MAX_KEEP_COMMAND_NUM; i++)
	{
		m_KeepButton[i].isUse = false;
		m_KeepButton[i].buttonType = BUTTON_TYPE_NONE;
	}
}

//=================================================================
// FINISHコマンドの始動ボタン表示
//=================================================================
void CCommandChart::isAppearFinishBeginCommand(void)
{
	if (!m_pCommandManager->GetCanUseFinishSkill(m_MyID))
	{
		m_apCommandName[MAX_COMAND_NAME_NUM - 1]->SetDrawFlag(false);
		m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetDrawFlag(false);
		if (m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->GetInputFlag())
		{
			for (int i = 0; i < MAX_COMAND_NUM; i++)
			{
				m_CommandInfo.commandList.finishAttack[i].pCommandUI->SetDrawFlag(false);
				m_CommandInfo.commandList.finishAttack[i].pCommandUI->SetInputFlag(false);
			}
		}
		m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);
		return;
	}
	m_apCommandName[MAX_COMAND_NAME_NUM - 1]->SetDrawFlag(true);
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetDrawFlag(true);
}

//=================================================================
// FINISH技完成してるか
//=================================================================
bool CCommandChart::isCompleteFinishSkill(void)
{
	// FINISH技の判定
	if (m_pCommandManager->GetCanUseFinishSkill(m_MyID))
	{
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			// 終端が押されてるかを調べる
			if (!m_CommandInfo.commandList.finishAttack[j].isEndList)
			{
				continue;
			}
			if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetInputFlag())
			{
				return true;
			}
			break;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// 全てのコマンドの後ろのポリゴンを消す
//-----------------------------------------------------------------------------
void CCommandChart::AllCandidateInputBackPolygonVanish(void)
{
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
	{
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// コマンドリストの初期化
	// 小技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// 中技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// 大技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// FINISH技
	// コマンドの長さ分回す
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 表示はしない
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}
}

//-----------------------------------------------------------------------------
// FINISH技のコマンドのボタン押されてるか
//-----------------------------------------------------------------------------
void CCommandChart::CheckPushForFinishCommand(void)
{
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// 始動ボタン押されてないなら判定しない
		if (!m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->GetInputFlag())
		{
			break;
		}

		if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// ボタン比較
		if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetButtonType() == m_aCommandKeep
			|| GetSameTimePushButton(m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetButtonType()))
		{
			m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.finishAttack[j].pCommandUI->InputUIUVChange(COMMAND_BUTTON_FINISHER[j + 1], true);

			// リストの最後のコマンドで無ければ次のコマンドにポリゴンを表示する
			if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.finishAttack[j].isEndList)
			{
				m_CommandInfo.commandList.finishAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.finishAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

		}
		break;
	}
}

//-----------------------------------------------------------------------------
// 何のボタン押されてるか
//-----------------------------------------------------------------------------
void CCommandChart::CheckPushCommand(void)
{
	bool isPushButton1 = false;
	bool isPushButton2 = false;
	bool isPushButton3 = false;
	bool isPushButton4 = false;

	//キー入力
	if (m_MyID == PLAYER_1)
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
	else if (m_MyID == PLAYER_2)
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

	// 同時押し判定のためにここで格納
	KeepPushButton(isPushButton1, isPushButton2, isPushButton3, isPushButton4);

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
	else
	{
		m_aCommandKeep = BUTTON_TYPE_NONE;
	}
}

// EOF