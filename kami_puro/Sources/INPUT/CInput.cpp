//=============================================================================
//
// CInputクラス [CInput.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CInput.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pDInput = 0;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CInput::CInput(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CInput::~CInput(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	// DirectInputオブジェクトの作成
	if(m_pDInput == NULL)
	{
		hr=DirectInput8Create(hInstance,
							  DIRECTINPUT_VERSION,
							  IID_IDirectInput8,
							  (void**)&m_pDInput,
							  NULL);
		if(FAILED(hr))
		{
			MessageBox(NULL,"DirectInputオブジェクトの作成失敗","DirectInputオブジェクトの作成失敗",MB_OK);
			return hr;
		}
	}

	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CInput::Uninit(void)
{
	if(m_pDInput != NULL)
	{
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CInput::Update(void)
{
}

//-----EOF----