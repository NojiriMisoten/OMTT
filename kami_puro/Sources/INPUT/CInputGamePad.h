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


//**********************************************
// 構造体定義
//**********************************************

// デバイス列挙関数に渡すデータを格納する構造体
typedef struct
{
	LPDIRECTINPUT8 pInput;				// デバイスを作成するためのインターフェイス
	LPDIRECTINPUTDEVICE8 *ppPadDevice;	// 使用するデバイスを格納するポインタのポインタ
	int padID;
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
	//**********************************************
	// マクロ
	//**********************************************
	static const int MAX_PAD_NUM = 2;						// コントローラーの最大数

	// 仮想コントローラーボタンの種類
	typedef enum
	{
		CONTROLLER_RIGHT_DOWN = 0,
		CONTROLLER_LEFT_DOWN,
		CONTROLLER_RIGHT_UP,
		CONTROLLER_LEFT_UP,
		CONTROLLER_START,
		CONTROLLER_DECIDE,				// 決定
		CONTROLLER_BUTTON_MAX
	}CONTROLLER_BUTTON;

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
	static bool GetGamePadPress(CONTROLLER_BUTTON key, int playerID);

	//======================================
	// ジョイパッドのトリガキー取得
	// 引数: 調べたいキー
	// 戻り値: 押された瞬間か
	//======================================
	static bool GetGamePadTrigger(CONTROLLER_BUTTON key, int playerID);

	//======================================
	// ジョイパッドのリリースキー取得
	// 引数: 調べたいキー
	// 戻り値: 離された瞬間か
	//======================================
	static bool GetGamePadRelease(CONTROLLER_BUTTON key, int playerID);

	//======================================
	// ジョイパッドのリピートキー取得
	// 引数: 調べたいキー
	// 戻り値: 一定時間押されっぱなしか
	//======================================
	static bool GetGamePadRepeat(CONTROLLER_BUTTON key, int playerID);

	//======================================
	// デモ用のキー入力保存
	// 引数: 何フレーム目か
	//======================================
	static void KeepKeyForDemo(int frame);

	//======================================
	// デモ用のキー入力保存
	// 引数: 押したキー、何フレーム目か
	//======================================
	static void SaveKeyForDemo(CONTROLLER_BUTTON key, int idx);

	// デモ用のキー入力ファイル出力
	static void OutPutKeyForDemo(void);

	// デモ用のキー読み込み
	static void LoadKeyForDemo(void);

	//======================================
	// デモ用のキー取得
	// 引数: 押したキー、何フレーム目か
	//======================================
	static bool GetKeyForDemo(CONTROLLER_BUTTON key, int idx);

	//======================================
	// デモ用のトリガーキー取得
	// 引数: 押したキー、何フレーム目か
	//======================================
	static bool GetTriggerKeyForDemo(CONTROLLER_BUTTON key, int idx);
	
	//======================================
	// デモ用のリリースキー取得
	// 引数: 押したキー、何フレーム目か
	//======================================
	static bool GetReleaseKeyForDemo(CONTROLLER_BUTTON key, int idx);

	//======================================
	// キーコフィング情報セット
	// 引数: コントローラーID, キーコフィング情報の配列アドレス
	//======================================
	static void SetKeyCofingInfo(int id, int* keyConfigInfo);

	//======================================
	// 何かトリガーしたか
	// 引数: コントローラーID, 押されたキーコード保存する場所
	// 戻り値：トリガーされてたか
	//======================================
	static bool CheckTriggerAnyKey(int ID, int* pCord);

	//======================================
	// コントローラー繋がってるか調べる
	// 引数: コントローラーID
	// 戻り値：繋がってるか
	//======================================
	static bool CheckConectPad(int ID);

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

	static GAMEPAD	m_GamePad[MAX_PAD_NUM];									// ゲームパッドの情報
	static GUID		m_pad_discrimination[MAX_PAD_NUM];						// 各デバイスの識別子を格納
	static bool		m_aKeyStateGamePad[MAX_PAD_NUM][KEY_MAX_NUM];			// ジョイパッドの押下状態を保持するワーク
	static bool		m_aKeyStateTriggerGamePad[MAX_PAD_NUM][KEY_MAX_NUM];	// ジョイパッドのトリガー状態を保持するワーク
	static bool		m_aKeyStateReleaseGamePad[MAX_PAD_NUM][KEY_MAX_NUM];	// ジョイパッドのリリース状態を保持するワーク
	static bool		m_aKeyStateRepeatGamePad[MAX_PAD_NUM][KEY_MAX_NUM];		// ジョイパッドのリピート状態を保持するワーク
	static int		m_aKeyStateRepeatCntGamePad[MAX_PAD_NUM][KEY_MAX_NUM];	// ジョイパッドのリピートカウンタ
	static unsigned short	m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];			// デモ用のキーのワーク
	static int				m_nKeyFrameForDemo;								// デモ用のキーフレーム
	
	static int		m_aKeyConfig[MAX_PAD_NUM][CONTROLLER_BUTTON_MAX];	// それぞれのキーコンフィグ情報
};

#endif
//----EOF----