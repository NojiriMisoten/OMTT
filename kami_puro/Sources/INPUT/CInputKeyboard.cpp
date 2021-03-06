//=============================================================================
//
// CInputKeyboardクラス [CInputKeyboard.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CInputKeyboard.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const BYTE KEY_CORD_INFO[KEYBOARD_CODE_MAX] =
{
	DIK_RETURN,
	DIK_M,
	DIK_RIGHT,
	DIK_LEFT,
	DIK_UP,
	DIK_DOWN,
	DIK_F3,			// コマンドチャートのアニメーションOPEN
	DIK_F4,			// コマンドチャートのアニメーションCLOSE
	DIK_LEFT,		// カットイン 左のプレイヤ
	DIK_RIGHT,		// カットイン 右のプレイヤ
	DIK_F5,			// フェードのUI
	DIK_Y,
	DIK_H,
	DIK_N,
	DIK_U,
	DIK_J,
	DIK_T,
	DIK_SEMICOLON,
	DIK_COLON,
	DIK_Q,
	DIK_A,
	DIK_W,
	DIK_S,
	DIK_I,
	DIK_K,
	DIK_O,
	DIK_L,
	DIK_1,
	DIK_2,
	DIK_3,
	DIK_4,
	DIK_5,
	DIK_SPACE,
	DIK_F1,
	DIK_F2,
	DIK_Z,				// プレイヤー1操作用（デバッグ）
	DIK_X,				// プレイヤー1操作用（デバッグ）
	DIK_E,				// プレイヤー1操作用（デバッグ）
	DIK_D,				// プレイヤー1操作用（デバッグ）
	DIK_C,				// プレイヤー1操作用（デバッグ）
	DIK_COMMA,			// プレイヤー2操作用（デバッグ）
	DIK_PERIOD,			// プレイヤー2操作用（デバッグ）
	DIK_P,				// プレイヤー2操作用（デバッグ）
	DIK_SEMICOLON,		// プレイヤー2操作用（デバッグ）
	DIK_SLASH,			// プレイヤー2操作用（デバッグ）
	DIK_F6,				// オーバーレイ(Readyとかの表示)
	DIK_F,
	DIK_F7,				// ロープタイマー
	DIK_F8,				// 時計を止める
	DIK_F9,				// 集中線を出す
	DIK_F12,
};

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
BYTE	CInputKeyboard :: m_aKeyState[KEY_NUM];
BYTE	CInputKeyboard :: m_TrigKeyState[KEY_NUM];
BYTE	CInputKeyboard :: m_ReleaseKeyState[KEY_NUM];
BYTE	CInputKeyboard :: m_RepeatKeyState[KEY_NUM];
int		CInputKeyboard :: m_RepeatKeyCount[KEY_NUM];
bool	CInputKeyboard :: m_Repeatflag[KEY_NUM];
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CInputKeyboard::CInputKeyboard(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CInputKeyboard::~CInputKeyboard(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 親クラス初期化
	CInput ::Init(hInstance, hWnd);

	HRESULT hr;
	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard
								, &m_pDIDevice
								, NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL,"デバイスオブジェクトの作成失敗","デバイスオブジェクトの作成失敗",MB_OK);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(NULL,"データフォーマットの設定失敗","データフォーマットの設定失敗",MB_OK);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(NULL,"協調モードの設定失敗","協調モードの設定失敗",MB_OK);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	// キーの初期化
	for(int i = 0; i < KEY_NUM; i++)
	{
		m_RepeatKeyCount[i] = 0;
		m_Repeatflag[i] = false;
		m_aKeyState[i] = 0;
		m_ReleaseKeyState[i] = 0;
		m_TrigKeyState[i] = 0;
		m_RepeatKeyState[i] = 0;
	}

	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CInputKeyboard::Uninit(void)
{
	if(m_pDIDevice != NULL)
	{
		m_pDIDevice->Unacquire();
		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[KEY_NUM];			// 最新のキー情報を受け取る
	
	// データを取得
	if(SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for(int nKey = 0; nKey < KEY_NUM; nKey++)
		{
			// トリガーキー取得
			m_TrigKeyState[nKey] = ((m_aKeyState[nKey] ^ aKeyState[nKey]) & aKeyState[nKey]);

			// リリースキー取得
			m_ReleaseKeyState[nKey] = ((m_aKeyState[nKey] ^ aKeyState[nKey]) & m_aKeyState[nKey]);

			// キー情報更新
			m_aKeyState[nKey] = aKeyState[nKey];
			
			m_RepeatKeyState[nKey] = (BYTE)false;
			// リピートキーフラグＯＮなら
			if(m_Repeatflag[(int)nKey] == true)
			{
				// カウントアップ
				m_RepeatKeyCount[(int)nKey]++;
			}
			// フラグOFFなら
			else if(m_Repeatflag[(int)nKey] == false)
			{
				// カウント初期化
				m_RepeatKeyCount[(int)nKey] = 0;
				if(GetKeyboardTrigger(nKey))
				{
					// フラグＯＮ
					m_Repeatflag[(int)nKey] = true;
					m_RepeatKeyState[nKey] = (BYTE)true;
				}
			}

			// 押されてなかったら
			if(!m_aKeyState[(int)nKey])
			{
				// フラグOFF
				m_Repeatflag[(int)nKey] = false;
			}
	
			// 一定時間押してたら
			if(m_RepeatKeyCount[(int)nKey] >= REPEAT_TIME)
			{
				// フラグON
				m_RepeatKeyState[nKey] = (BYTE)true;
			}
		}
	}
	// 失敗したら
	else
	{
		// 権利取得
		m_pDIDevice->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(BYTE key)
{
	for(int nKey = 0; nKey < KEY_NUM; nKey++)
	{
		if(m_aKeyState[(int)key])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(BYTE key)
{
	int nKey;
	for(nKey = 0; nKey < KEY_NUM; nKey++)
	{
		if(m_TrigKeyState[(int)key])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRepeat(BYTE key)
{
	if(m_RepeatKeyState[(int)key])
	{
		return true;
	}
	return false;
}

//=============================================================================
// キーボードのリリ−ス状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(BYTE key)
{
	for(int nKey = 0; nKey < KEY_NUM; nKey++)
	{
		if(m_ReleaseKeyState[(int)key])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "キーコードが不正");
#endif
	if (m_aKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}

	return false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "キーコードが不正");
#endif
	if (m_TrigKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	
	return false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRepeat(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "キーコードが不正");
#endif
	if (m_RepeatKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	return false;
}

//=============================================================================
// キーボードのリリ−ス状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "キーコードが不正");
#endif
	if (m_ReleaseKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	
	return false;
}
//----EOF----