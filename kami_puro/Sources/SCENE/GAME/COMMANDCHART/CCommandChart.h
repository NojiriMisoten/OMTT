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
#include "../PLAYER/CPlayerManager.h"

class CCommandChartManager;
//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
// コマンドの最大入力数
static const int MAX_COMMAND_KEEP = 6;
// 次の入力候補の技の最大数
static const int MAX_NEXT_COMMAND_VIEW = 4;

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
	COMMAND_TYPE_NONE = -1,		// コマンドが不正だった時
	COMMAND_TYPE_CHOP = 0,		// チョップ
	COMMAND_TYPE_ELBOW,			// エルボー
	COMMAND_TYPE_LARIAT,		// ラリアット
	COMMAND_TYPE_ROLLING,		// ローリングエルボー
	COMMAND_TYPE_SHOULDER,		// フライングエルボー
	COMMAND_TYPE_DROPKICK,		// ドロップキック
	COMMAND_TYPE_SLAP,			// ビンタ
	COMMAND_TYPE_BACKDROP,		// バックドロップ
	COMMAND_TYPE_STUNNER,		// スタナー
	COMMAND_TYPE_ROPE,			// ロープ
	COMMAND_TYPE_FINISHER,		// 決め技
	COMMAND_TYPE_MAX			// 技の数

}COMMAND_TYPE;

// 技の大別した種類
typedef enum
{
	SKILL_SMALL_ATTACK = 0,
	SKILL_MIDDLE_ATTACK,
	SKILL_BIG_ATTACK,
	SKILL_MAX,
	SKILL_FINISH_ATTACK	// MAXが固定の数として使っているため
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
	static const int MAX_BEGIN_COMAND_NUM = 5;
	// コマンドチャートの状態
	typedef enum
	{
		MODE_APPEAR = 0,			// 出現中
		MODE_INPUT,					// 入力受付中
		MODE_COMPLETE_COMMAND,		// コマンド完成
		MODE_VANISH,				// 表示オフ中
		MODE_RESET,
		MODE_ROPE,
		MODE_MAX
	}MODE_COMMAND_CHART;

	// コンストラクタ
	// 引数：デバイス、プレイヤー番号
	CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager);

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
	static CCommandChart* Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager);

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
	CCommandChart::MODE_COMMAND_CHART GetCommandChartMode(void) { return m_CommandChartMode; };

	//================================================================
	// 始動コマンドだけの状態になる
	// 技出した後バトルモード継続ならこっち
	//================================================================
	void ResetCommandList(void);

	//================================================================
	// 位置やテクスチャなどの何回も呼べる初期化
	// MODE_APPEARにセットする前に呼んでほしい
	//================================================================
	void ResetAllCommand(void);

	//********************************************
	//    2015/12/4
	//    佐藤　追記
	//********************************************
	// 現在表示している全てのコマンドの後ろに表示してあるコマンドを消す
	//    コマンドチャートの必要が無くなった時に使用
	void AllCandidateInputBackPolygonVanish(void);
	//********************************************
	//    2015/12/4
	//    佐藤　追記
	//********************************************


private:
	static const int MAX_COMAND_NUM = 5;
	static const int MAX_COMAND_NAME_NUM = 5;
	//*******************追記開始11/23　野尻 **************************************
	// コマンド情報の基礎
	typedef struct
	{
		CCommandChartUI*	pCommandUI;
		bool				isEndList;
		D3DXVECTOR3			vAnimationPosDest;
	}BASE_COMMAND;

	// 始動ボタン
	typedef struct
	{
		BASE_COMMAND firstCommand[MAX_BEGIN_COMAND_NUM];
	}BEGIN_COMMAND;

	// コマンドリスト
	typedef struct
	{
		BASE_COMMAND smallAttack[MAX_COMAND_NUM];	// 始動ボタンを除く個数
		BASE_COMMAND middleAttack[MAX_COMAND_NUM];
		BASE_COMMAND largeAttack[MAX_COMAND_NUM];
		BASE_COMMAND finishAttack[MAX_COMAND_NUM];
	}COMMAND_LIST;

	// コマンド全体
	typedef struct
	{
		BEGIN_COMMAND	beginCommand;
		COMMAND_LIST	commandList;
	}COMMAND_STRUCT;

	typedef struct
	{
		BUTTON_TYPE	buttonType;
		bool		isUse;
	}KEEP_BUTTON_INFO;
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

	// 右下キー開始のコマンドチャートの生成
	void CreateRightDownTechnicCommand(void);

	// FINISHコマンドチャートの生成
	void CreateFinishTechnicCommand(void);

	// コマンドUIが押された状態にするか判定して押されている状態にするのであれば押された状態にする
	void CommandUIInput(BUTTON_TYPE button);

	// ロープコマンドをセット
	void SetRopeCommand(void);

	// FINISHコマンドの始動ボタン表示するか
	void isAppearFinishBeginCommand(void);

	// FINISH技完成してるか
	bool isCompleteFinishSkill(void);

	//*******************追記開始11/23　野尻 **************************************
	// 始動コマンドの作成 
	// 初期化時のみ呼ぶこと！！
	void InitCreateBeginCommand(void);
	
	// 次に入力する候補のコマンドの作成 
	// 初期化時のみ呼ぶこと！！
	void InitCreateCommandList(void);

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

	// 技完成時選択されてない技を消す
	void VanishOtherSkill(SKILL_TYPE completeSkill);

	// 押されたボタン保持関数
	void KeepPushButton(bool isPushRightUp, bool isPushRightDown, bool isPushLeftUp, bool isPushLeftDown);

	//=================================================================
	// 同時押し判定関数
	// BUTTON_TYPE_5(上同時押し)、BUTTON_TYPE_6(下同時押し)
	// 以外は引数で入れてもfalseしか返しません
	//=================================================================
	bool GetSameTimePushButton(BUTTON_TYPE type);

	// 保持してたコマンド破棄
	void RefleshKeepCommand();
	//*******************追記終了11/23　野尻 **************************************
	static const int MAX_KEEP_COMMAND_NUM = 10;

	//*************************************
	// 変数
	//*************************************
	// コマンド保持用変数
	BUTTON_TYPE m_aCommandKeep;
	KEEP_BUTTON_INFO m_KeepButton[MAX_KEEP_COMMAND_NUM];
	// デバイスの保持
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// 発生候補の技名表示用UIのポインタの保持
	CCommandName* m_apCommandName[MAX_COMAND_NAME_NUM];
	// コマンドチャートの背面に生成するポリゴン
	CScene2D* m_pBackPolygon;
	// 入力後のUIを表示するx座標
	float m_fPosX;
	// 表示しているコマンドを消すまでのカウント
	int m_nCommandDeathCnt;
	// 保持中のコマンド数
	int m_nKeepCommandNum;
	// 自分のID
	PLAYER_ID  m_MyID;
	// コマンド入力可能かどうかのフラグ
	bool m_isCommandInput;

	// コマンドリスト
	COMMAND_STRUCT	m_CommandInfo;

	// コマンドチャートの状態
	MODE_COMMAND_CHART	m_CommandChartMode;

	// 完成したコマンド
	COMMAND_TYPE		m_CompleteCommand;
	// 完成した送る予定コマンド
	COMMAND_TYPE		m_DestCompleteCommand;

	// 完成後待ち時間管理用（FINSH技の同時押しの猶予待つため）
	int		m_WiatCompleteCommandTimer;
	
	// 完成したスキル
	SKILL_TYPE	m_CompleteSkill;

	//================================================================
	// 閉じたり開いたりアニメーション　塚本
	//================================================================
	// アニメーションをする際に必要なデータ
	struct CAnimeData
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3 m_PosDest;
		float m_Width;
		float m_Height;
		float m_WidthDest;
		float m_HeightDest;
		// この関数で初期化してアニメする
		void Init(
			D3DXVECTOR3 pos, D3DXVECTOR3 posDest,
			float width, float widthDest,
			float height, float heightDest)
		{
			m_Pos = pos;
			m_PosDest = posDest;
			m_Width = width;
			m_Height = height;
			m_WidthDest = widthDest;
			m_HeightDest = heightDest;
		}
		// 0~1のタイムで保管した幅
		float GetEasingWidth(float time)
		{
			return EasingInterpolation(m_Width, m_WidthDest, time);
		}
		// 0~1のタイムで保管した高さ
		float GetEasingHeight(float time)
		{
			return EasingInterpolation(m_Height, m_HeightDest, time);
		}
		// 0~1のタイムで保管した座標X
		float GetEasingPosX(float time)
		{
			return EasingInterpolation(m_Pos.x, m_PosDest.x, time);
		}
		// 0~1のタイムで保管した座標Y
		float GetEasingPosY(float time)
		{
			return EasingInterpolation(m_Pos.y, m_PosDest.y, time);
		}
	};
	// アニメーション用の初期化関数 このクラスのInitで呼ぶ
	void InitAnime();
	// 更新で呼ぶ。アニメーションの更新まとめ
	void UpdateAnime();
	// 開いたり閉じたりのアニメーション開始関数
	void StartAnimeClose();
	void StartAnimeOpen();

	// アニメーションをするかフラグ
	bool m_isAnime;
	// 開くアニメーションの方か アニメが終わった時の遷移に必要
	bool m_isAnimeOpen;
	// アニメーション用の保管するときに使うタイム
	float m_AnimeCount;
	// アニメーションデータ　背景のポリゴン
	CAnimeData m_Back;
	// アニメーションデータ　小攻撃
	CAnimeData m_CommandSmall[MAX_COMAND_NUM];
	// アニメーションデータ　中攻撃
	CAnimeData m_CommandMiddle[MAX_COMAND_NUM];
	// アニメーションデータ　大攻撃
	CAnimeData m_CommandLarge[MAX_COMAND_NUM];
	// アニメーションデータ　FINISH攻撃
	CAnimeData m_CommandFinish[MAX_COMAND_NUM];
	// アニメーションデータ　わかんない
	CAnimeData m_CommandFirst[MAX_BEGIN_COMAND_NUM];
	// アニメーションデータ　技名
	CAnimeData m_CommandName[MAX_COMAND_NAME_NUM];
	// コマンドマネージャーのアドレス
	CCommandChartManager*	m_pCommandManager;
};

#endif

//EOF