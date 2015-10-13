//=============================================================================
//
// CInputGamePadクラス [CInputGamePad.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CINPUTGAMEPAD_H_
#define _CINPUTGAMEPAD_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInput.h"

//**********************************************
// マクロ
//**********************************************

// パッドの方向キー用
#define PP_NONE 0x0
#define PP_UP 0x1
#define PP_RIGHT 0x2
#define PP_DOWN 0x4
#define PP_LEFT 0x8

// ELECOMパッドのボタン用
static const int ELECOM_PAD_BUTTON_1 = 0;
static const int ELECOM_PAD_BUTTON_2 = 1;
static const int ELECOM_PAD_BUTTON_3 = 2;
static const int ELECOM_PAD_BUTTON_4 = 3;
static const int ELECOM_PAD_BUTTON_5 = 4;
static const int ELECOM_PAD_BUTTON_6 = 5;
static const int ELECOM_PAD_BUTTON_7 = 6;
static const int ELECOM_PAD_BUTTON_8 = 7;
static const int ELECOM_PAD_BUTTON_9 = 8;
static const int ELECOM_PAD_BUTTON_10 = 9;
static const int ELECOM_PAD_BUTTON_11 = 10;
static const int ELECOM_PAD_BUTTON_12 = 11;

// XBOXパッドのボタン用
static const int XBOX_PAD_BUTTON_A = 0;
static const int XBOX_PAD_BUTTON_B = 1;
static const int XBOX_PAD_BUTTON_X = 2;
static const int XBOX_PAD_BUTTON_Y = 3;
static const int XBOX_PAD_BUTTON_LB = 4;
static const int XBOX_PAD_BUTTON_RB = 5;
static const int XBOX_PAD_BUTTON_BACK = 6;
static const int XBOX_PAD_BUTTON_START = 7;
static const int XBOX_PAD_BUTTON_L3 = 8;
static const int XBOX_PAD_BUTTON_R3 = 9;

//**********************************************
// 構造体定義
//**********************************************

// デバイス列挙関数に渡すデータを格納する構造体
typedef struct
{
	LPDIRECTINPUT8 pInput;				// デバイスを作成するためのインターフェイス
	LPDIRECTINPUTDEVICE8 *ppPadDevice;	// 使用するデバイスを格納するポインタのポインタ
}STR_ENUMDATA;

// コントローラーの構造体
typedef struct
{
	LPDIRECTINPUTDEVICE8	pPadDevice;				// デバイス
	GUID					Controller_ID;			// コントローラーのＩＤ
	DIJOYSTATE2				PadData;				// 今回のパッドのデータ
	DIJOYSTATE2				OldPadData;				// 前回のパッドのデータ
	BYTE					DirectryData;			// 今回の方向データ
	BYTE					OldDirectryData;		// 前回の方向データ
}GAMEPAD;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInputGamePad : CInput
{
public:
	// ボタンの種類
	typedef enum
	{
		LEFT_STICK_LEFT = 0,	// ←
		LEFT_STICK_RIGHT,		// →
		LEFT_STICK_UP,			// ↑
		LEFT_STICK_DOWN,		// ↓
		RIGHT_STICK_LEFT,		// ←
		RIGHT_STICK_RIGHT,		// →
		RIGHT_STICK_UP,			// ↑
		RIGHT_STICK_DOWN,		// ↓
		KEY_1,					// □
		KEY_2,					// ○
		KEY_3,					// ×
		KEY_4,					// △
		KEY_5,					// L1
		KEY_6,					// R1
		KEY_7,					// L2
		KEY_8,					// Start
		KEY_9,					// LeftStick
		KEY_10,					// RightStick
		KEY_11,					// Select
		KEY_12,					// 
		KEY_DECIDE,				// □△×○
		KEY_MAX
	}KEY;

	// コンストラクタ
	CInputGamePad(void);

	// デストラクタ
	~CInputGamePad(void);

	//============================================
	// 初期化
	// 引数: インスタンス、ウインドウハンドル
	// 戻り値: 成否
	//============================================
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);
	
	// コールバック関数
	static BOOL CALLBACK EnumGamePad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext);
	static BOOL CALLBACK EnumObject(LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef);

	//======================================
	// ジョイパッドのプレスキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static bool GetGamePadPress(KEY key);

	//======================================
	// ジョイパッドのトリガキー取得
	// 引数: 調べたいキー
	// 戻り値: 押された瞬間か
	//======================================
	static bool GetGamePadTrigger(KEY key);

	//======================================
	// ジョイパッドのリリースキー取得
	// 引数: 調べたいキー
	// 戻り値: 離された瞬間か
	//======================================
	static bool GetGamePadRelease(KEY key);

	//======================================
	// ジョイパッドのリピートキー取得
	// 引数: 調べたいキー
	// 戻り値: 一定時間押されっぱなしか
	//======================================
	static bool GetGamePadRepeat(KEY key);

	//======================================
	// デモ用のキー入力保存
	// 引数: 何フレーム目か
	//======================================
	static void KeepKeyForDemo(int frame);

	//======================================
	// デモ用のキー入力保存
	// 引数: 押したキー、何フレーム目か
	//======================================
	static void SaveKeyForDemo(KEY key, int idx);

	// デモ用のキー入力ファイル出力
	static void OutPutKeyForDemo(void);

	// デモ用のキー読み込み
	static void LoadKeyForDemo(void);

	//======================================
	// デモ用のキー取得
	// 引数: 押したキー、何フレーム目か
	//======================================
	static bool GetKeyForDemo(KEY key, int idx);

	//======================================
	// デモ用のトリガーキー取得
	// 引数: 押したキー、何フレーム目か
	//======================================
	static bool GetTriggerKeyForDemo(KEY key, int idx);
	
	//======================================
	// デモ用のリリースキー取得
	// 引数: 押したキー、何フレーム目か
	//======================================
	static bool GetReleaseKeyForDemo(KEY key, int idx);

private:
	// デモ用キーの保存場所マップ
	typedef enum
	{
		L_UP_KEY	= 0x0001,
		L_DOWN_KEY	= 0x0002,
		L_LEFT_KEY	= 0x0004,
		L_RIGHT_KEY = 0x0008,
		JUMP_KEY	= 0x0010,
		KICK_KEY	= 0x0020,
		R_UP_KEY	= 0x0040,
		R_DOWN_KEY	= 0x0080,
		R_LEFT_KEY	= 0x0100,
		R_RIGHT_KEY = 0x0200,
		R_STICK_KEY = 0x0400,
	}KEEP_KEY;

	// ジョイパッドのキー情報設定
	static void SetKeyStateGamePad(void);

	static const int INPUT_DEEP_X = 15;						// アナログスティックの受け取り範囲
	static const int INPUT_DEEP_Y = 290;					// アナログスティックの受け取り範囲
	static const int MAX_KEEP_KEY_FOR_DEMO = 1800;			// デモの最大フレーム数
	static const int m_kInputDeep				= 8;		// 入力深度
	static const int m_KeyStateMask				= 0x0080;	// キー状態マスク用定数
	static const int m_KeyMaxNum				= 256;		// キーの総数
	static const int m_KeyRepeatInputStartTime	= 30;		// リピートキーの入力開始時間
	static const int m_KeyRepeateIntervalTime	= 1;		// リピートキー間隔の時間
	static const int KEY_MAX_NUM = 256;						// キーの総数

	static GAMEPAD	m_GamePad;										// ゲームパッドの情報
	static GUID		m_pad_discrimination;							// 各デバイスの識別子を格納
	static bool		m_aKeyStateGamePad[KEY_MAX_NUM];				// ジョイパッドの押下状態を保持するワーク
	static bool		m_aKeyStateTriggerGamePad[KEY_MAX_NUM];			// ジョイパッドのトリガー状態を保持するワーク
	static bool		m_aKeyStateReleaseGamePad[KEY_MAX_NUM];			// ジョイパッドのリリース状態を保持するワーク
	static bool		m_aKeyStateRepeatGamePad[KEY_MAX_NUM];			// ジョイパッドのリピート状態を保持するワーク
	static int		m_aKeyStateRepeatCntGamePad[KEY_MAX_NUM];		// ジョイパッドのリピートカウンタ
	static unsigned short	m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];	// デモ用のキーのワーク
	static int				m_nKeyFrameForDemo;						// デモ用のキーフレーム
};

#endif
//----EOF----