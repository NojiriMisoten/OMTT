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
static const BYTE KEY_CORD_INFO[KEYBOARD_CORD_MAX] =
{
	DIK_RETURN,
	DIK_W,
	DIK_S,
	DIK_D,
	DIK_A,
	DIK_M,
	DIK_SPACE,
	DIK_RIGHT,
	DIK_LEFT,
	DIK_UP,
	DIK_DOWN,
	DIK_I,
	DIK_K,
	DIK_O,
	DIK_U,
	DIK_Q,
	DIK_W,
	DIK_A,
	DIK_S,
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
// キーボードのリリ－ス状態を取得
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
bool CInputKeyboard::GetKeyboardPress(KEYBOARD_CORD key)
{
	assert(key >= 0 && key < KEYBOARD_CORD_MAX && "キーコードが不正");
	if (m_aKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}

	return false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CORD key)
{
	assert(key >= 0 && key < KEYBOARD_CORD_MAX && "キーコードが不正");
	if (m_TrigKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	
	return false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRepeat(KEYBOARD_CORD key)
{
	assert(key >= 0 && key < KEYBOARD_CORD_MAX && "キーコードが不正");
	if (m_RepeatKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	return false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(KEYBOARD_CORD key)
{
	assert(key >= 0 && key < KEYBOARD_CORD_MAX && "キーコードが不正");
	
	if (m_ReleaseKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	
	return false;
}
//----EOF----