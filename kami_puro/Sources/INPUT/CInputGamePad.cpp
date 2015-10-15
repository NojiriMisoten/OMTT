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
static const int	JUDGE_UP_MIN_STICK = 0 * 100;					// 上判定のスティックの最少傾き
static const int	JUDGE_UP_MAX_STICK = 45 * 100;					// 上判定のスティックの最大傾き
static const int	JUDGE_RIGHT_MIN_STICK = 45 * 100;				// 右判定のスティックの最少傾き
static const int	JUDGE_RIGHT_MAX_STICK = 135 * 100;				// 右判定のスティックの最大傾き
static const int	JUDGE_DOWN_MIN_STICK = 135 * 100;				// 下判定のスティックの最少傾き
static const int	JUDGE_DOWN_MAX_STICK = 225 * 100;				// 下判定のスティックの最大傾き
static const int	JUDGE_LEFT_MIN_STICK = 225 * 100;				// 左判定のスティックの最少傾き
static const int	JUDGE_LEFT_MAX_STICK = 315 * 100;				// 左判定のスティックの最大傾き
static const int	JUDGE_RIGHT_UP_MIN_STICK = 5 * 100;				// 右上判定のスティックの最少傾き
static const int	JUDGE_RIGHT_UP_MAX_STICK = 85 * 100;			// 右上判定のスティックの最大傾き
static const int	JUDGE_RIGHT_DOWN_MIN_STICK = 95 * 100;			// 右下判定のスティックの最少傾き
static const int	JUDGE_RIGHT_DOWN_MAX_STICK = 175 * 100;			// 右下判定のスティックの最大傾き
static const int	JUDGE_LEFT_DOWN_MIN_STICK = 185 * 100;			// 左下判定のスティックの最少傾き
static const int	JUDGE_LEFT_DOWN_MAX_STICK = 265 * 100;			// 左下判定のスティックの最大傾き
static const int	JUDGE_LEFT_UP_MIN_STICK = 275 * 100;			// 左上判定のスティックの最少傾き
static const int	JUDGE_LEFT_UP_MAX_STICK = 355 * 100;			// 左上判定のスティックの最大傾き
static const int	JUDGE_MAX_STICK = 360 * 100;					// スティックの最大傾き

//**********************************************
// スタティックメンバ変数
//**********************************************
GUID	CInputGamePad::m_pad_discrimination[MAX_PAD_NUM];
GAMEPAD	CInputGamePad::m_GamePad[MAX_PAD_NUM];
bool	CInputGamePad::m_aKeyStateGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateTriggerGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateReleaseGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateRepeatGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
int		CInputGamePad::m_aKeyStateRepeatCntGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
unsigned short	CInputGamePad::m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];
int		CInputGamePad::m_nKeyFrameForDemo;
int		CInputGamePad::m_aKeyConfig[MAX_PAD_NUM][CONTROLLER_BUTTON_MAX];
//==============================================
// コンストラクタ
//==============================================
CInputGamePad::CInputGamePad(void)
{
	m_nKeyFrameForDemo = 0;

	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		for (int cnt = 0; cnt < KEY_MAX_NUM; ++cnt)
		{
			m_aKeyStateGamePad[idx][cnt] = false;
			m_aKeyStateTriggerGamePad[idx][cnt] = false;
			m_aKeyStateReleaseGamePad[idx][cnt] = false;
			m_aKeyStateRepeatGamePad[idx][cnt] = false;
		}
		for (int i = 0; i < CONTROLLER_BUTTON_MAX; ++i)
		{
			m_aKeyConfig[idx][i] = 0;
		}
	}
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

	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		ed.ppPadDevice = &m_GamePad[idx].pPadDevice;
		ed.padID = idx;

		hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL	// ゲームコントローラーが対象
			, EnumGamePad			// 列挙する関数
			, &ed					// 列挙関数に渡したいデータはここに入れる
			, DIEDFL_ATTACHEDONLY);	// インストール・接続済みのデバイスのみ取得

		// ゲームパッドの入力情報はDIJOYSTATE2に格納されるので
		// データフォーマットにはc_dfDIJoystick2を指定
		if (m_GamePad[idx].pPadDevice != NULL)
		{
			// アナログキーのデータを設定
			// ２番目の引数にpPadDeviceを指定しておくと、
			// EnumObject関数のpvRefに、このpPadDeviceが格納されるようになるため、
			// pPadDeviceをスタティックやグローバル変数にしなくて済む。
			m_GamePad[idx].pPadDevice->EnumObjects(EnumObject, m_GamePad[idx].pPadDevice, DIDFT_AXIS);

			m_GamePad[idx].pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			hr = m_GamePad[idx].pPadDevice->SetDataFormat(&c_dfDIJoystick2);

			// 失敗したら開放
			if (FAILED(hr))
			{
				if (m_GamePad[idx].pPadDevice != NULL)
				{
					m_GamePad[idx].pPadDevice->Release();
					m_GamePad[idx].pPadDevice = NULL;
				}
			}

			hr = m_GamePad[idx].pPadDevice->SetDataFormat(&c_dfDIJoystick2);

			// 失敗したら開放
			if (FAILED(hr))
			{
				if (m_GamePad[idx].pPadDevice != NULL)
				{
					m_GamePad[idx].pPadDevice->Release();
					m_GamePad[idx].pPadDevice = NULL;
				}
			}

			hr = m_GamePad[idx].pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

			// 失敗したら開放
			if (FAILED(hr))
			{
				MessageBox(NULL, "協調モードの設定失敗", "協調モードの設定失敗", MB_OK);
				return hr;
			}

			// コントローラーへのアクセス権を獲得(入力制御開始)
			m_GamePad[idx].pPadDevice->Acquire();
		}
		m_GamePad[idx].DirectryData = m_GamePad[idx].OldDirectryData = PP_NONE;
	}

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
	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		if (m_GamePad[idx].pPadDevice)
		{
			m_GamePad[idx].pPadDevice->Unacquire();
			m_GamePad[idx].pPadDevice->Release();
			m_GamePad[idx].pPadDevice = NULL;
		}
	}
}

//==============================================
// 更新
//==============================================
void CInputGamePad::Update(void)
{
	HRESULT hr;
	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		bool aKeyStateOld[KEY_MAX_NUM];

		if (!m_GamePad[idx].pPadDevice)
		{
			continue;
		}

		// 前回のデータを保存
		for (int pos = 0; pos < 127; pos++)
		{
			m_GamePad[idx].OldPadData.rgbButtons[pos] = m_GamePad[idx].PadData.rgbButtons[pos];
		}

		for (int nCntKey = 0; nCntKey < KEY_MAX_NUM; nCntKey++)
		{
			aKeyStateOld[nCntKey] = m_aKeyStateGamePad[idx][nCntKey];
		}

		// デバイスからデータを取得
		hr = m_GamePad[idx].pPadDevice->GetDeviceState(sizeof(m_GamePad[idx].PadData), &m_GamePad[idx].PadData);
		if (SUCCEEDED(hr))
		{
			// キー情報設定
			SetKeyStateGamePad();
			for (int nCntKey = 0; nCntKey < KEY_MAX_NUM; nCntKey++)
			{
				// トリガーキー更新
				m_aKeyStateTriggerGamePad[idx][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[idx][nCntKey]) & m_aKeyStateGamePad[idx][nCntKey];

				// リリースキー更新
				m_aKeyStateReleaseGamePad[idx][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[idx][nCntKey]) & !m_aKeyStateGamePad[idx][nCntKey];

				// リピートキー更新
				m_aKeyStateRepeatGamePad[idx][nCntKey] = m_aKeyStateTriggerGamePad[idx][nCntKey];
				if (m_aKeyStateGamePad[idx][nCntKey])
				{
					m_aKeyStateRepeatCntGamePad[idx][nCntKey]++;
					if (m_aKeyStateRepeatCntGamePad[idx][nCntKey] >= JUDGE_TIME_REPEAT_KEY)
					{
						m_aKeyStateRepeatGamePad[idx][nCntKey] = m_aKeyStateGamePad[idx][nCntKey];
					}
				}
				else
				{
					m_aKeyStateRepeatCntGamePad[idx][nCntKey] = 0;
					m_aKeyStateRepeatGamePad[idx][nCntKey] = 0;
				}
			}
		}
		else
		{
			// 権利獲得
			m_GamePad[idx].pPadDevice->Acquire();
		}
	#ifdef _DEBUG
		for (int pos = 0; pos < 127; pos++)
		{
			if (m_GamePad[idx].PadData.rgbButtons[pos] != 0)
			{
				CDebugProc::Print("プレイヤーID：%d\n", idx);
				CDebugProc::Print("ボタンコード：%d\n", pos);
			}
		}
	#endif
	}
}

//==============================================
// コールバック関数
//==============================================
BOOL CALLBACK CInputGamePad::EnumGamePad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext)
{
	STR_ENUMDATA *ed = (STR_ENUMDATA*)pContext;
	DIDEVCAPS diDevCaps;							// ジョイパッドの能力情報

	for (int idx = 0; idx < ed->padID; ++idx)
	{
		// 既に取得済みのデバイスだった場合はもう一回実行する
		if (m_pad_discrimination[idx] == pInstance->guidInstance)
		{
			return DIENUM_CONTINUE;
		}
	}

	// デバイスの識別子を保存
	m_pad_discrimination[ed->padID] = pInstance->guidInstance;
	

	HRESULT hr;
	
	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = ed->pInput->CreateDevice(pInstance->guidInstance, ed->ppPadDevice, NULL);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;  // デバイスが作成できないので列挙を続ける
	}
	
	// ジョイパッドの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	
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
	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		if (!m_GamePad[idx].pPadDevice)
		{
			continue;
		}

		// ゲームパッドボタン[1]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_RIGHT_DOWN]])
		{
			m_aKeyStateGamePad[idx][CONTROLLER_RIGHT_DOWN] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][CONTROLLER_RIGHT_DOWN] = false;
		}
		// ゲームパッドボタン[2]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_LEFT_DOWN]])
		{
			m_aKeyStateGamePad[idx][CONTROLLER_LEFT_DOWN] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][CONTROLLER_LEFT_DOWN] = false;
		}
		// ゲームパッドボタン[3]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_RIGHT_UP]])
		{
			m_aKeyStateGamePad[idx][CONTROLLER_RIGHT_UP] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][CONTROLLER_RIGHT_UP] = false;
		}
		// ゲームパッドボタン[4]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_LEFT_UP]])
		{
			m_aKeyStateGamePad[idx][CONTROLLER_LEFT_UP] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][CONTROLLER_LEFT_UP] = false;
		}
		// ゲームパッドボタン[5]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_START]])
		{
			m_aKeyStateGamePad[idx][CONTROLLER_START] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][CONTROLLER_START] = false;
		}

		// ゲームパッドボタンA,Y,X,R,L,STARTのいずれか
		bool isPushDecideKey = m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_RIGHT_DOWN]]
							|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_LEFT_DOWN]]
							|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_RIGHT_UP]]
							|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][CONTROLLER_LEFT_UP]];

		if (isPushDecideKey)
		{
			m_aKeyStateGamePad[idx][CONTROLLER_DECIDE] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][CONTROLLER_DECIDE] = false;
		}
	}
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadPress(CONTROLLER_BUTTON key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateGamePad[playerID][key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadTrigger(CONTROLLER_BUTTON key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateTriggerGamePad[playerID][key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadRelease(CONTROLLER_BUTTON key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateReleaseGamePad[playerID][key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputGamePad::GetGamePadRepeat(CONTROLLER_BUTTON key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateRepeatGamePad[playerID][key];
}

//=============================================================================
// デモ用のキー入力保存
//=============================================================================
void CInputGamePad::KeepKeyForDemo(int frame)
{
	for(int idx = 0; idx < CONTROLLER_BUTTON_MAX; ++idx)
	{
		if(m_aKeyStateGamePad[idx])
		{
			SaveKeyForDemo((CONTROLLER_BUTTON)idx, frame);
		}
	}
}

//=============================================================================
// デモ用のキー入力保存
//=============================================================================
void CInputGamePad::SaveKeyForDemo(CONTROLLER_BUTTON key, int idx)
{
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	//{
	//	return;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_LEFT_KEY;
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_RIGHT_KEY;
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_UP_KEY;
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_DOWN_KEY;
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_LEFT_KEY;
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_RIGHT_KEY;
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_UP_KEY;
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_DOWN_KEY;
	//}
	//
	//if(key == KEY_3)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | JUMP_KEY;
	//}
	//
	//if(key == KEY_1)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | KICK_KEY;
	//}
	//
	//if(key == KEY_10)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_STICK_KEY;
	//}
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
bool CInputGamePad::GetKeyForDemo(CONTROLLER_BUTTON key, int idx)
{
	//// 限界値判定
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	//{
	//	return false;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	if(m_nKeepDemoKey[idx] & L_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	if(m_nKeepDemoKey[idx] & R_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_3)
	//{
	//	if(m_nKeepDemoKey[idx] & JUMP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_1)
	//{
	//	if(m_nKeepDemoKey[idx] & KICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_10)
	//{
	//	if(m_nKeepDemoKey[idx] & R_STICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

//=============================================================================
// デモ用のトリガーキー取得
//=============================================================================
bool CInputGamePad::GetTriggerKeyForDemo(CONTROLLER_BUTTON key, int idx)
{
	//// 限界値判定
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	//{
	//	return false;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & L_UP_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & R_UP_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_3)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & JUMP_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & JUMP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_1)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & KICK_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & KICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_10)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// 前回押されてたら
	//		if(m_nKeepDemoKey[idx - 1] & R_STICK_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_STICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

//=============================================================================
// デモ用のリリースキー取得
//=============================================================================
bool CInputGamePad::GetReleaseKeyForDemo(CONTROLLER_BUTTON key, int idx)
{
	//// 限界値判定
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx <= 0)
	//{
	//	return false;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_LEFT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_RIGHT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & L_UP_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_UP_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_DOWN_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_LEFT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_RIGHT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & R_UP_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_UP_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_DOWN_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_3)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & JUMP_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & JUMP_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_1)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & KICK_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & KICK_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_10)
	//{
	//	// 前回押されてないなら
	//	if(!(m_nKeepDemoKey[idx - 1] & R_STICK_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_STICK_KEY))
	//	{
	//		return true;
	//	}
	//}
	return false;
}

//======================================
// キーコフィング情報セット
//======================================
void CInputGamePad::SetKeyCofingInfo(int id, int* keyConfigInfo)
{
	if (id < 0 || id >= MAX_PAD_NUM)
	{
		return;
	}
	for (int i = 0; i < CONTROLLER_BUTTON_MAX; ++i)
	{
		m_aKeyConfig[id][i] = keyConfigInfo[i];
	}
}

//======================================
// 何かトリガーしたか
//======================================
bool CInputGamePad::CheckTriggerAnyKey(int ID, int* pCord)
{
	if (ID < 0 || ID >= MAX_PAD_NUM)
	{
		return false;
	}

	if (!m_GamePad[ID].pPadDevice)
	{
		return false;
	}
	for (int pos = 0; pos < 127; pos++)
	{
		if (m_GamePad[ID].OldPadData.rgbButtons[pos] != 0)
		{
			continue;
		}
		if (m_GamePad[ID].PadData.rgbButtons[pos] != 0)
		{
			*pCord = pos;
			return true;
		}
	}

	return false;
}

//======================================
// コントローラー繋がってるか調べる
//======================================
bool CInputGamePad::CheckConectPad(int ID)
{
	if (ID >= MAX_PAD_NUM || ID < 0)
	{
		return true;
	}

	if (!m_GamePad[ID].pPadDevice)
	{
		return false;
	}
	return true;
}

//----EOF----