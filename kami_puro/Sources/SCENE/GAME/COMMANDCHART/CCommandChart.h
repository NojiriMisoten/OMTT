//=============================================================================
//
//	CCommandChartクラスの宣言(CCommandChart.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHART_H__
#define __CCOMMNADCHART_H__

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
// 技の種類(弱中強の３種類のこと)
static const int COMMAND_TYPE_NUM = 3;

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

private:
	//*************************************
	// 関数
	//*************************************
	// デフォルトの状態に変数を戻す
	void SetDefault(void);

	// コマンド入力
	void InputCommand(void);

	// コマンドのリセット
	void ResetCommand(void);

	// 次に表示するコマンドを削除
	void DeathNextCommand(void);

	// 一番初めに表示する入力候補のコマンドの作成
	void CreateFirstCommand(void);
	
	// 次に入力する候補のコマンドのチェック
	// 戻り値：入力候補となるコマンド情報の配列のポインタ
	COMMAND_INFO* NextCommandCheck(void);

	// 次に入力する候補のコマンドの作成
	// 引数：何個目のコマンド表記か
	void CreateNextCommand(int nNumCommand);

	// コマンドのチェック
	// 引数：技候補の構造体の配列
	// 戻り値：コマンドが正しかったか否か
	bool CheckCommand(COMMAND_INFO* Technic);

	// 右上キー開始のコマンドチャートの生成
	void CreateRightUpTechnicCommand(int nNumCommand);

	// 左上キー開始のコマンドチャートの生成
	void CreateLeftUpTechnicCommand(int nNumCommand);

	// 左下キー開始のコマンドチャートの生成
	void CreateLeftDownTechnicCommand(int nNumCommand);

	// コマンドUIが押された状態にするか判定して押されている状態にするのであれば押された状態にする
	void CommandUIInput(BUTTON_TYPE button);

	//*************************************
	// 変数
	//*************************************
	// コマンド保持用変数
	BUTTON_TYPE m_aCommandKeep;
	// デバイスの保持
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// 表示する入力されたコマンドUIの保持
	//CCommandChartUI* m_apCommandUI[MAX_COMMAND_KEEP];
	// 次入力候補のコマンドUIの保持
	CCommandChartUI* m_apNextCommandUI[MAX_NEXT_COMMAND_VIEW];
	// 発生候補の技名表示用UIのポインタの保持
	CCommandName* m_apCommandName[MAX_NEXT_COMMAND_VIEW];
	// コマンドチャートの背面に生成するポリゴン
	CScene2D* m_pBackPolygon;
	// コマンドUIのリストの最初のポインタ
	COMMAND_UI_INFO* m_pCommandUI[COMMAND_TYPE_NUM];
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
};

#endif

//EOF