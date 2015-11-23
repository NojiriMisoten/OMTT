//=============================================================================
//
//	CCommandChartクラスの宣言(CCommandChart.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHART_H__
#define __CCOMMANDCHART_H__

//-----------------------------------------------------------------------------
//	インクルード
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "CCommandChartUI.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
// コマンドの最大入力数
static const int MAX_COMMAND_KEEP = 6;
// 次の入力候補の技の最大数
static const int MAX_NEXT_COMMAND_VIEW = 4;
static const int MY_ID_1 = 0;	// ID
static const int MY_ID_2 = 1;	// ID
// プレイヤーの数
static const int MAX_PLAYER = 2;

// コマンドの入力数
// 初期技
static const int COMMAND_INPUT_NUM_MONO = 1;
// 小技
static const int COMMAND_INPUT_NUM_SMALL = 3;
// 中技
static const int COMMAND_INPUT_NUM_MIDDLE = 4;
// 大技
static const int COMMAND_INPUT_NUM_LARGE = 5;
// 決め技
static const int COMMAND_INPUT_NUM_FINISHER = 6;

// 技の種類
typedef enum
{
	COMMAND_TYPE_CHOP = 0,		// チョップ
	COMMAND_TYPE_ELBOW,			// エルボー
	COMMAND_TYPE_LARIAT,		// ラリアット
	COMMAND_TYPE_ROLLING_ELBOW,	// ローリングエルボー
	COMMAND_TYPE_FLYING_ELBOW,	// フライングエルボー
	COMMAND_TYPE_DROP_KICK,		// ドロップキック
	COMMAND_TYPE_FACE_SLAPPING,	// ビンタ
	COMMAND_TYPE_BACKDROP,		// バックドロップ
	COMMAND_TYPE_STANER,		// スタナー
	COMMAND_TYPE_ROPE,			// ロープ
	COMMAND_TYPE_FINISHER,		// 決め技
	COMMAND_TYPE_MAX,			// 技の数
	COMMAND_TYPE_NONE			// コマンドが不正だった時
}COMMAND_TYPE;

// 技の大別した種類
typedef enum
{
	SKILL_SMALL_ATTACK = 0,
	SKILL_MIDDLE_ATTACK,
	SKILL_BIG_ATTACK,
	SKILL_MAX
}SKILL_TYPE;

//-----------------------------------------------------------------------------
//	構造体定義
//-----------------------------------------------------------------------------
// コマンド情報
struct COMMAND_INFO
{
	const int m_nCommandLength;
	const COMMAND_TYPE m_CommandType;
	const BUTTON_TYPE* m_Command;
};

// コマンドUIの保存情報
struct COMMAND_UI_INFO
{
	COMMAND_UI_INFO* m_NextCommand;		// 次の候補のボタン情報
	CCommandChartUI* m_pUIType;			// コマンドチャートUIのポインタの保持
	bool m_isInputButton;				// このボタンの入力確認
};

//-----------------------------------------------------------------------------
//	前方宣言
//-----------------------------------------------------------------------------
class CCommandName;

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class CCommandChart
{
public:
	// コマンドチャートの状態
	typedef enum
	{
		MODE_APPEAR = 0,			// 出現中
		MODE_INPUT,					// 入力受付中
		MODE_COMPLETE_COMMAND,		// コマンド完成
		MODE_VANISH,				// 表示オフ中
		MODE_RESET,
		MODE_MAX
	}MODE_COMMAND_CHART;

	// コンストラクタ
	// 引数：デバイス、プレイヤー番号
	CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID);

	// デストラクタ
	~CCommandChart();

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 終了
	void Uninit(void);

	// 自身の生成
	// 引数：デバイスのポインタ、自分のプレイヤー番号
	// 戻り値：自身のポインタ、プレイヤー番号
	static CCommandChart* Create(LPDIRECT3DDEVICE9* pDevice, int nID);

	// 画面外へのフェードアウト
	void ScreenOut(void);

	// 画面内へのフェードイン
	void ScreenIn(void);

	// 最終的に繰り出す技を調べて返す
	// 戻り値：繰り出す技のタイプ
	COMMAND_TYPE UseTechnic(void);

	// コマンド入力可能フラグの変更
	// 引数：コマンド入力可能にするのならばtrue不可にするのならばfalse
	void SetCommandInputFlag(bool isInput){ m_isCommandInput = isInput; }

	void SetCommandChartMode(MODE_COMMAND_CHART mode){ m_CommandChartMode = mode; };

private:
	//*******************追記開始11/23　野尻 **************************************
	// コマンド情報の基礎
	typedef struct
	{
		CCommandChartUI*	pCommandUI;
		bool				isEndList;
	}BASE_COMMAND;

	// 始動ボタン
	typedef struct
	{
		BASE_COMMAND firstCommand[MAX_NEXT_COMMAND_VIEW];
	}BEGIN_COMMAND;

	// コマンドリスト
	typedef struct
	{
		BASE_COMMAND smallAttack[COMMAND_INPUT_NUM_SMALL - 1];	// 始動ボタンを除く個数
		BASE_COMMAND middleAttack[COMMAND_INPUT_NUM_MIDDLE - 1];
		BASE_COMMAND largeAttack[COMMAND_INPUT_NUM_LARGE - 1];
	}COMMAND_LIST;

	// コマンド全体
	typedef struct
	{
		BEGIN_COMMAND	beginCommand;
		COMMAND_LIST	commandList;
	}COMMAND_STRUCT;

	//*******************追記終了11/23　野尻 **************************************

	//*************************************
	// 関数
	//*************************************
	// コマンド入力
	void InputCommand(void);

	// コマンドのチェック
	void CheckCommand(void);

	// 右上キー開始のコマンドチャートの生成
	void CreateRightUpTechnicCommand(void);

	// 左上キー開始のコマンドチャートの生成
	void CreateLeftUpTechnicCommand(void);

	// 左下キー開始のコマンドチャートの生成
	void CreateLeftDownTechnicCommand(void);

	// コマンドUIが押された状態にするか判定して押されている状態にするのであれば押された状態にする
	void CommandUIInput(BUTTON_TYPE button);

	//*******************追記開始11/23　野尻 **************************************
	// 始動コマンドの作成 
	// 初期化時のみ呼ぶこと！！
	void InitCreateBeginCommand(void);
	
	// 次に入力する候補のコマンドの作成 
	// 初期化時のみ呼ぶこと！！
	void InitCreateCommandList(void);

	//================================================================
	// 始動コマンドだけの状態になる
	// 描画フラグ変えるだけ
	//================================================================
	void ResetCommandList(void);

	//================================================================
	// 位置やテクスチャなどの何回も呼べる初期化
	// 始動コマンドだけの状態になる
	// 基本、座標を変えて描画フラグ変えるだけ
	//================================================================
	void ResetAllCommand(void);

	// コマンドチャート表示
	void AppearanceCommandChart(void);

	// コマンドチャート消す
	void VanishCommandChart(void);

	//================================================================
	// 次に入力する候補のコマンドのリセット
	// テクスチャ変えてるだけ
	// コマンドのタイプはその時自動で変わる(InputUIUVChange()で変えてる)
	// 一個目押された後に一回呼ぶ
	//================================================================
	void ResetNextCommand(void);

	// コマンド消す
	void VanishCommand(void);

	//*******************追記終了11/23　野尻 **************************************

	//*************************************
	// 変数
	//*************************************
	// コマンド保持用変数
	BUTTON_TYPE m_aCommandKeep;
	// デバイスの保持
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// 発生候補の技名表示用UIのポインタの保持
	CCommandName* m_apCommandName[MAX_NEXT_COMMAND_VIEW];
	// コマンドチャートの背面に生成するポリゴン
	CScene2D* m_pBackPolygon;
	// 入力後のUIを表示するx座標
	float m_fPosX;
	// 表示しているコマンドを消すまでのカウント
	int m_nCommandDeathCnt;
	// 保持中のコマンド数
	int m_nKeepCommandNum;
	// 自分のID
	int m_MyID;
	// コマンド入力可能かどうかのフラグ
	bool m_isCommandInput;

	//*******************追記開始11/23　野尻 **************************************
	// コマンドリスト
	COMMAND_STRUCT	m_CommandInfo;

	// コマンドチャートの状態
	MODE_COMMAND_CHART	m_CommandChartMode;

	// 完成したコマンド
	COMMAND_TYPE		m_CompleteCommand;
	//*******************追記終了11/23　野尻 **************************************
};

#endif

//EOF