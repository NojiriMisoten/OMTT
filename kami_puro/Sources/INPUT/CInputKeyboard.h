//=============================================================================
//
// CInputKeyboardクラス [CInputKeyboard.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CINPUTKEYBOARD_H_
#define _CINPUTKEYBOARD_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInput.h"

//*****************************************************************************
// キーボードの入力列挙体
//*****************************************************************************
typedef enum
{
	KEYBOARD_CODE_DECIDE = 0,
	KEYBOARD_CODE_PLAYER_1_FORWORD,
	KEYBOARD_CODE_PLAYER_1_BACK,
	KEYBOARD_CODE_PLAYER_1_ROT_RIGHT,
	KEYBOARD_CODE_PLAYER_1_ROT_LEFT,
	KEYBOARD_CODE_UI_START_TEST,
	KEYBOARD_CODE_UI_USE_STAMINA_TEST,
	KEYBOARD_CODE_UI_UP_CROWD_RIGHT_TEST,
	KEYBOARD_CODE_UI_UP_CROWD_LEFT_TEST,
	KEYBOARD_CODE_UI_UP_HP_TEST,
	KEYBOARD_CODE_UI_DOWN_HP_TEST,
	KEYBOARD_CODE_CAMERA_ROT_UP,
	KEYBOARD_CODE_CAMERA_ROT_DOWN,
	KEYBOARD_CODE_CAMERA_ROT_LEFT,
	KEYBOARD_CODE_CAMERA_ROT_RIGHT,
	KEYBOARD_CODE_CAMERA_SMALL_SHAKE,
	KEYBOARD_CODE_CAMERA_BIG_SHAKE,
	KEYBOARD_CODE_CAMERA_MOVE,
	KEYBOARD_CODE_CAMERA_SET,
	KEYBOARD_CODE_COMMAND_DEBUG_Q,	// コマンドチャートのデバッグ用
	KEYBOARD_CODE_COMMAND_DEBUG_W,	// コマンドチャートのデバッグ用
	KEYBOARD_CODE_COMMAND_DEBUG_A,	// コマンドチャートのデバッグ用
	KEYBOARD_CODE_COMMAND_DEBUG_S,	// コマンドチャートのデバッグ用
	KEYBOARD_CODE_MAX
}KEYBOARD_CODE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInputKeyboard : CInput
{
public:
	// コンストラクタ
	CInputKeyboard(void);

	// デストラクタ
	~CInputKeyboard(void);

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

	//======================================
	// キーボードのプレスキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardPress(KEYBOARD_CODE key);

	//======================================
	// キーボードのトリガーキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardTrigger(KEYBOARD_CODE key);

	//======================================
	// キーボードのリピートキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardRepeat(KEYBOARD_CODE key);

	//======================================
	// キーボードのリリースキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardRelease(KEYBOARD_CODE key);

private:
	//======================================
	// キーボードのプレスキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardPress(BYTE key);

	//======================================
	// キーボードのトリガーキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardTrigger(BYTE key);

	//======================================
	// キーボードのリピートキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardRepeat(BYTE key);

	//======================================
	// キーボードのリリースキー取得
	// 引数: 調べたいキー
	// 戻り値: 押されてるか
	//======================================
	static	bool GetKeyboardRelease(BYTE key);

	static const int KEY_NUM = 256;								//キーの数
	static const int REPEAT_TIME = 30;							//リピートキーが反応する時間
	
	static	BYTE	m_aKeyState[KEY_NUM];						//キーボードの押下状態を受け取るワーク
	static	BYTE	m_TrigKeyState[KEY_NUM];					//トリガーキー用
	static	BYTE	m_ReleaseKeyState[KEY_NUM];					//リリースキー用
	static	BYTE	m_RepeatKeyState[KEY_NUM];					//リピート用
	static	int		m_RepeatKeyCount[KEY_NUM];					//リピートキー用
	static	bool	m_Repeatflag[KEY_NUM];						//リピートキー用フラグ
};

#endif
//----EOF----