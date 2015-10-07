//=============================================================================
//
// コントローラー入力処理 [CInputGamePad.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//**********************************************
// インクルード
//**********************************************
#include "CInputGamePad.h"
#include "../DEBUG_PROC/CDebugProc.h"
#include <stdio.h>

//**********************************************
// マクロ
//**********************************************
static const char*	DEMO_FILE_PATH = "data/DEMO_KEY/demo_key.txt";	// デモ用のキーの保存パス
static const int	JUDGE_TIME_REPEAT_KEY = 20;						// リピートキーの判定時間
static const int	JUDGE_LEFT_MIN_STICK = 225 * 100;				// 左判定のスティックの最少傾き
static const int	JUDGE_LEFT_MAX_STICK = 315 * 100;				// 左判定のスティックの最大傾き
static const int	JUDGE_RIGHT_MIN_STICK = 45 * 100;				// 右判定のスティックの最少傾き
static const int	JUDGE_RIGHT_MAX_STICK = 135 * 100;				// 右判定のスティックの最大傾き
static const int	JUDGE_UP_MIN_STICK = 0 * 100;					// 上判定のスティックの最少傾き
static const int	JUDGE_UP_MAX_STICK = 45 * 100;					// 上判定のスティックの最大傾き
static const int	JUDGE_DOWN_MIN_STICK = 135 * 100;				// 下判定のスティックの最少傾き
static const int	JUDGE_DOWN_MAX_STICK = 225 * 100;				// 下判定のスティックの最大傾き
static const int	JUDGE_MAX_STICK = 360 * 100;					// スティックの最大傾き

//**********************************************
// スタティックメンバ変数
//**********************************************
GUID	CInputGamePad::m_pad_discrimination;
GAMEPAD	CInputGamePad::m_GamePad;
bool	CInputGamePad::m_aKeyStateGamePad[KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateTriggerGamePad[KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateReleaseGamePad[KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateRepeatGamePad[KEY_MAX_NUM];
int		CInputGamePad::m_aKeyStateRepeatCntGamePad[KEY_MAX_NUM];
unsigned short	CInputGamePad::m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];
int		CInputGamePad::m_nKeyFrameForDemo;
//==============================================
// コンストラクタ
//==============================================
CInputGamePad::CInputGamePad(void)
{
	m_nKeyFrameForDemo = 0;
}

//==============================================
// デストラクタ
//==============================================
CInputGamePad::~CInputGamePad(void)
{
}

//==============================================
// 初期化
//==============================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance,HWND hWnd)
{

	// インターフェイスの取得
	HRESULT hr;  
	hr = DirectInput8Create(hInstance				// ソフトのインスタンスハンドル
							, DIRECTINPUT_VERSION	// DirectInputのバージョン
							, IID_IDirectInput8		// 取得するインターフェイスのタイプ
							, (LPVOID*)&m_pDInput	// インターフェイスの格納先
							, NULL);				// COM集成の制御オブジェクト（使わないのでNULL）
	if(FAILED(hr))
	{
		MessageBox(NULL,"DirectInputオブジェクトの作成失敗","DirectInputオブジェクトの作成失敗",MB_OK);
		return hr;
	}

	// ジョイパッドの作成
	// コールバック関数に転送したいデータを格納
	STR_ENUMDATA ed;
	ed.pInput = m_pDInput;
	ed.ppPadDevice = &m_GamePad.pPadDevice;

	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL	// ゲームコントローラーが対象
								, EnumGamePad			// 列挙する関数
								, &ed					// 列挙関数に渡したいデータはここに入れる
								, DIEDFL_ATTACHEDONLY);	// インストール・接続済みのデバイスのみ取得

	// ゲームパッドの入力情報はDIJOYSTATE2に格納されるので
	// データフォーマットにはc_dfDIJoystick2を指定
	if(m_GamePad.pPadDevice != NULL)
	{
		// アナログキーのデータを設定
		// ２番目の引数にpPadDeviceを指定しておくと、
		// EnumObject関数のpvRefに、このpPadDeviceが格納されるようになるため、
		// pPadDeviceをスタティックやグローバル変数にしなくて済む。
		m_GamePad.pPadDevice->EnumObjects(EnumObject, m_GamePad.pPadDevice, DIDFT_AXIS);

		m_GamePad.pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		hr = m_GamePad.pPadDevice->SetDataFormat(&c_dfDIJoystick2);
				
		// 失敗したら開放
		if(FAILED(hr))
		{
			if(m_GamePad.pPadDevice != NULL)
			{
				m_GamePad.pPadDevice->Release();
				m_GamePad.pPadDevice = NULL;
			}
		}

		hr = m_GamePad.pPadDevice->SetDataFormat(&c_dfDIJoystick2);

		// 失敗したら開放
		if(FAILED(hr))
		{
			if(m_GamePad.pPadDevice != NULL)
			{
				m_GamePad.pPadDevice->Release();
				m_GamePad.pPadDevice = NULL;
			}
		}

		hr = m_GamePad.pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		// 失敗したら開放
		if(FAILED(hr))
		{
			MessageBox(NULL,"協調モードの設定失敗","協調モードの設定失敗",MB_OK);
			return hr;
		}

		// コントローラーへのアクセス権を獲得(入力制御開始)
		m_GamePad.pPadDevice->Acquire();
	}
	m_GamePad.DirectryData = m_GamePad.OldDirectryData = PP_NONE;

	// デモ用配列の初期化
	for(int idx = 0; idx < MAX_KEEP_KEY_FOR_DEMO; idx++)
	{
		m_nKeepDemoKey[idx] = 0x0000;
	}
	return S_OK;
}

//==============================================
// 終了
//==============================================
void CInputGamePad::Uninit(void)
{
	if(m_GamePad.pPadDevice)
	{
		m_GamePad.pPadDevice->Unacquire();
		m_GamePad.pPadDevice->Release();
		m_GamePad.pPadDevice = NULL;
	}
}

//==============================================
// 更新
//==============================================
void CInputGamePad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[KEY_MAX];

	if(!m_GamePad.pPadDevice)
	{
		return;
	}

	// 前回のデータを保存
	for(int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
	{
		aKeyStateOld[nCntKey] = m_aKeyStateGamePad[nCntKey];
	}

	// デバイスからデータを取得
	hr = m_GamePad.pPadDevice->GetDeviceState(sizeof(m_GamePad.PadData), &m_GamePad.PadData);
	if(SUCCEEDED(hr))
	{
		// キー情報設定
		SetKeyStateGamePad();
		for(int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
		{
			// トリガーキー更新
			m_aKeyStateTriggerGamePad[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[nCntKey]) & m_aKeyStateGamePad[nCntKey];
			
			// リリースキー更新
			m_aKeyStateReleaseGamePad[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[nCntKey]) & !m_aKeyStateGamePad[nCntKey];
			
			// リピートキー更新
			m_aKeyStateRepeatGamePad[nCntKey] = m_aKeyStateTriggerGamePad[nCntKey];
			if(m_aKeyStateGamePad[nCntKey])
			{
				m_aKeyStateRepeatCntGamePad[nCntKey]++;
				if(m_aKeyStateRepeatCntGamePad[nCntKey] >= JUDGE_TIME_REPEAT_KEY)
				{
					m_aKeyStateRepeatGamePad[nCntKey] = m_aKeyStateGamePad[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCntGamePad[nCntKey] = 0;
				m_aKeyStateRepeatGamePad[nCntKey] = 0;
			}
		}
	}
	else
	{
		// 権利獲得
		m_GamePad.pPadDevice->Acquire();
	}
#ifdef _DEBUG
	for(int pos = 0; pos < 127; pos++)
	{
		if(m_GamePad.PadData.rgbButtons[pos] != 0)
		{
			CDebugProc::Print("ボタンコード：%d",pos);
		}
	}
#endif
}

//==============================================
// コールバック関数
//==============================================
BOOL CALLBACK CInputGamePad::EnumGamePad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext)
{
	STR_ENUMDATA *ed = (STR_ENUMDATA*)pContext;
	DIDEVCAPS diDevCaps;							// ジョイパッドの能力情報

	// 既に取得済みのデバイスだった場合はもう一回実行する
	if(m_pad_discrimination == pInstance->guidInstance)
	{
		return DIENUM_CONTINUE;
	}
	m_pad_discrimination = pInstance->guidInstance;

	HRESULT hr;
	
	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = ed->pInput->CreateDevice(pInstance->guidInstance, ed->ppPadDevice, NULL);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;  // デバイスが作成できないので列挙を続ける
	}
	
	// ジョイパッドの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	
	// デバイスの識別子を保存
	m_pad_discrimination = pInstance->guidInstance;

	// 希望するデバイスが作成できたので列挙を終了する
	return DIENUM_STOP;
}

BOOL CALLBACK CInputGamePad::EnumObject(LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef)
{
	DIPROPRANGE range;
	range.diph.dwSize = sizeof(DIPROPRANGE);
	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range.diph.dwObj = pInstance->dwType;			// ボタンやスティックのデータ
	range.diph.dwHow = DIPH_BYID;					// タイプで取得することを指定
	range.lMin = -10;								// 値の最小値
	range.lMax = +10;								// 値の最大値

	LPDIRECTINPUTDEVICE8 pInputDev = (LPDIRECTINPUTDEVICE8)pvRef;
	pInputDev->SetProperty(DIPROP_RANGE, &range.diph);

	return DIENUM_CONTINUE;
}

//=============================================================================
// ジョイパッドのキー情報設定
//=============================================================================
void CInputGamePad::SetKeyStateGamePad(void)
{
	// ゲームパッドボタン左スティック[左キー & スティック左]
	if(m_GamePad.PadData.lX <= -m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_LEFT_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_LEFT_MAX_STICK))
	{
		m_aKeyStateGamePad[LEFT_STICK_LEFT] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_LEFT] = false;
	}
	// ゲームパッドボタン左スティック[右キー & スティック右]
	if(m_GamePad.PadData.lX >= m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_RIGHT_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_RIGHT_MAX_STICK))
	{
		m_aKeyStateGamePad[LEFT_STICK_RIGHT] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_RIGHT] = false;
	}
	// ゲームパッドボタン左スティック[上キー & スティック上]
	if(m_GamePad.PadData.lY <= -m_kInputDeep
	|| ((m_GamePad.PadData.rgdwPOV[0] >= JUDGE_LEFT_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_LEFT_MAX_STICK)
		|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_UP_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_UP_MAX_STICK)))
	{
		m_aKeyStateGamePad[LEFT_STICK_UP] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_UP] = false;
	}
	// ゲームパッドボタン左スティック[下キー & スティック下]
	if(m_GamePad.PadData.lY >= m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_DOWN_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_DOWN_MAX_STICK))
	{
		m_aKeyStateGamePad[LEFT_STICK_DOWN] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_DOWN] = false;
	}


	// ゲームパッドボタン右スティック[右キー & スティック左]
	if(m_GamePad.PadData.lRx <= -m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[1] >= JUDGE_LEFT_MAX_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_LEFT_MAX_STICK))
	{
		m_aKeyStateGamePad[RIGHT_STICK_LEFT] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_LEFT] = false;
	}
	// ゲームパッドボタン右スティック[右キー & スティック右]
	if(m_GamePad.PadData.lRx >= m_kInputDeep
		|| (m_GamePad.PadData.rgdwPOV[1] >= JUDGE_RIGHT_MIN_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_RIGHT_MAX_STICK))
	{
		m_aKeyStateGamePad[RIGHT_STICK_RIGHT] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_RIGHT] = false;
	}
	// ゲームパッドボタン右スティック[上キー & スティック上]
	if(m_GamePad.PadData.lRy <= -m_kInputDeep
		|| ((m_GamePad.PadData.rgdwPOV[1] >= JUDGE_LEFT_MAX_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_MAX_STICK)
		|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_UP_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_UP_MAX_STICK)))
	{
		m_aKeyStateGamePad[RIGHT_STICK_UP] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_UP] = false;
	}
	// ゲームパッドボタン右スティック[下キー & スティック下]
	if(m_GamePad.PadData.lRy >= m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[1] >= JUDGE_DOWN_MIN_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_DOWN_MAX_STICK))
	{
		m_aKeyStateGamePad[RIGHT_STICK_DOWN] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_DOWN] = false;
	}

	// ゲームパッドボタン[1]
	if(m_GamePad.PadData.rgbButtons[0])
	{
		m_aKeyStateGamePad[KEY_1] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_1] = false;
	}
	// ゲームパッドボタン[2]
	if(m_GamePad.PadData.rgbButtons[1])
	{
		m_aKeyStateGamePad[KEY_2] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_2] = false;
	}
	// ゲームパッドボタン[3]
	if(m_GamePad.PadData.rgbButtons[2])
	{
		m_aKeyStateGamePad[KEY_3] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_3] = false;
	}
	// ゲームパッドボタン[4]
	if(m_GamePad.PadData.rgbButtons[3])
	{
		m_aKeyStateGamePad[KEY_4] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_4] = false;
	}
	// ゲームパッドボタン[5]
	if(m_GamePad.PadData.rgbButtons[4])
	{
		m_aKeyStateGamePad[KEY_5] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_5] = false;
	}
	// ゲームパッドボタン[6]
	if(m_GamePad.PadData.rgbButtons[5])
	{
		m_aKeyStateGamePad[KEY_6] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_6] = false;
	}
	// ゲームパッドボタン[7]
	if(m_GamePad.PadData.rgbButtons[6])
	{
		m_aKeyStateGamePad[KEY_7] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_7] = false;
	}
	// ゲームパッドボタン[8]
	if(m_GamePad.PadData.rgbButtons[7])
	{
		m_aKeyStateGamePad[KEY_8] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_8] = false;
	}
	// ゲームパッドボタン[9]
	if(m_GamePad.PadData.rgbButtons[8])
	{
		m_aKeyStateGamePad[KEY_9] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_9] = false;
	}
	// ゲームパッドボタン[10]
	if(m_GamePad.PadData.rgbButtons[9])
	{
		m_aKeyStateGamePad[KEY_10] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_10] = false;
	}
	// ゲームパッドボタン[11]
	if(m_GamePad.PadData.rgbButtons[10])
	{
		m_aKeyStateGamePad[KEY_11] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_11] = false;
	}
	// ゲームパッドボタン[12]
	if(m_GamePad.PadData.rgbButtons[11])
	{
		m_aKeyStateGamePad[KEY_12] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_12] = false;
	}

	// ゲームパッドボタン[1][2][3][4]のいずれか
	if(m_GamePad.PadData.rgbButtons[0]
	|| m_GamePad.PadData.rgbButtons[1]
	|| m_GamePad.PadData.rgbButtons[2]
	|| m_GamePad.PadData.rgbButtons[3])
	{
		m_aKeyStateGamePad[KEY_DECIDE] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_DECIDE] = false;
	}
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadPress(KEY key)
{
	return m_aKeyStateGamePad[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadTrigger(KEY key)
{
	return m_aKeyStateTriggerGamePad[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadRelease(KEY key)
{
	return m_aKeyStateReleaseGamePad[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadRepeat(KEY key)
{
	return m_aKeyStateRepeatGamePad[key];
}

//=============================================================================
// デモ用のキー入力保存
//=============================================================================
void CInputGamePad::KeepKeyForDemo(int frame)
{
	for(int idx = 0; idx < KEY_MAX_NUM; ++idx)
	{
		if(m_aKeyStateGamePad[idx])
		{
			SaveKeyForDemo((KEY)idx, frame);
		}
	}
}

//=============================================================================
// デモ用のキー入力保存
//=============================================================================
void CInputGamePad::SaveKeyForDemo(KEY key, int idx)
{
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	{
		return;
	}

	if(key == LEFT_STICK_LEFT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_LEFT_KEY;
	}

	if(key == LEFT_STICK_RIGHT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_RIGHT_KEY;
	}

	if(key == LEFT_STICK_UP)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_UP_KEY;
	}

	if(key == LEFT_STICK_DOWN)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_DOWN_KEY;
	}

	if(key == RIGHT_STICK_LEFT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_LEFT_KEY;
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_RIGHT_KEY;
	}

	if(key == RIGHT_STICK_UP)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_UP_KEY;
	}

	if(key == RIGHT_STICK_DOWN)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_DOWN_KEY;
	}

	if(key == KEY_3)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | JUMP_KEY;
	}

	if(key == KEY_1)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | KICK_KEY;
	}

	if(key == KEY_10)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_STICK_KEY;
	}
}

//=============================================================================
// デモ用のキー入力ファイル出力
//=============================================================================
void CInputGamePad::OutPutKeyForDemo(void)
{
	// 書き込み用ファイル
	FILE* fpw = fopen(DEMO_FILE_PATH, "wb");

	// 失敗したら
	if(fpw == NULL)
	{
		printf("ファイル読み込み失敗\n");
		rewind(stdin);
		getchar();
	}

	// 成功したら
	else
	{
		// キー情報出力
		fwrite(&m_nKeepDemoKey[0], sizeof(m_nKeepDemoKey), 1, fpw);
	}
	fclose(fpw);
}

//=============================================================================
// デモ用のキー入力ファイルロード
//=============================================================================
void CInputGamePad::LoadKeyForDemo(void)
{
	// 読み込み用ファイル
	FILE* fpr = fopen(DEMO_FILE_PATH, "rb");

	// 失敗したら
	if(fpr == NULL)
	{
		printf("ファイル読み込み失敗\n");
		rewind(stdin);
		getchar();
	}

	// 成功したら
	else
	{
		// キー情報取得
		fread(&m_nKeepDemoKey[0], sizeof(m_nKeepDemoKey), 1, fpr);
	}
	fclose(fpr);
}

//=============================================================================
// デモ用のキー取得
//=============================================================================
bool CInputGamePad::GetKeyForDemo(KEY key, int idx)
{
	// 限界値判定
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	{
		return false;
	}

	if(key == LEFT_STICK_LEFT)
	{
		if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_RIGHT)
	{
		if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_UP)
	{
		if(m_nKeepDemoKey[idx] & L_UP_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_DOWN)
	{
		if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_LEFT)
	{
		if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_UP)
	{
		if(m_nKeepDemoKey[idx] & R_UP_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_DOWN)
	{
		if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == KEY_3)
	{
		if(m_nKeepDemoKey[idx] & JUMP_KEY)
		{
			return true;
		}
	}

	if(key == KEY_1)
	{
		if(m_nKeepDemoKey[idx] & KICK_KEY)
		{
			return true;
		}
	}

	if(key == KEY_10)
	{
		if(m_nKeepDemoKey[idx] & R_STICK_KEY)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// デモ用のトリガーキー取得
//=============================================================================
bool CInputGamePad::GetTriggerKeyForDemo(KEY key, int idx)
{
	// 限界値判定
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	{
		return false;
	}

	if(key == LEFT_STICK_LEFT)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_RIGHT)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_UP)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & L_UP_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_UP_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_DOWN)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_LEFT)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_UP)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & R_UP_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_UP_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_DOWN)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == KEY_3)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & JUMP_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & JUMP_KEY)
		{
			return true;
		}
	}

	if(key == KEY_1)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & KICK_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & KICK_KEY)
		{
			return true;
		}
	}

	if(key == KEY_10)
	{
		if(idx - 1 >= 0)
		{
			// 前回押されてたら
			if(m_nKeepDemoKey[idx - 1] & R_STICK_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_STICK_KEY)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// デモ用のリリースキー取得
//=============================================================================
bool CInputGamePad::GetReleaseKeyForDemo(KEY key, int idx)
{
	// 限界値判定
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx <= 0)
	{
		return false;
	}

	if(key == LEFT_STICK_LEFT)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_LEFT_KEY))
		{
			return true;
		}
	}

	if(key == LEFT_STICK_RIGHT)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_RIGHT_KEY))
		{
			return true;
		}
	}

	if(key == LEFT_STICK_UP)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & L_UP_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_UP_KEY))
		{
			return true;
		}
	}

	if(key == LEFT_STICK_DOWN)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_DOWN_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_LEFT)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_LEFT_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_RIGHT_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_UP)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & R_UP_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_UP_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_DOWN)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_DOWN_KEY))
		{
			return true;
		}
	}

	if(key == KEY_3)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & JUMP_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & JUMP_KEY))
		{
			return true;
		}
	}

	if(key == KEY_1)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & KICK_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & KICK_KEY))
		{
			return true;
		}
	}

	if(key == KEY_10)
	{
		// 前回押されてないなら
		if(!(m_nKeepDemoKey[idx - 1] & R_STICK_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_STICK_KEY))
		{
			return true;
		}
	}
	return false;
}

//----EOF----