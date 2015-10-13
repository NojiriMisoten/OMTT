//=============================================================================
//
// デバッグ情報処理 [CDebugProc.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../RENDERER/CRenderer.h"
#include "CDebugProc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//*****************************************************************************
// マクロ
//*****************************************************************************
static const RECT DRAW_RECT = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};		// 描画範囲
static const D3DXCOLOR DRAW_TEXT_COLOR = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);	// 文字の色

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
LPD3DXFONT	CDebugProc::m_pD3DXFONT = NULL;						// フォントへのポインタ
char		CDebugProc::m_aStr[LENGTH_STRING_BUFF] = {'\0'};	// デバッグ表示用の文字列のバッファ
bool		CDebugProc::m_bDisp = false;						// デバッグ表示ON/OFF
int			CDebugProc::m_nCounter = 0;							// 文字数カウンター
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CDebugProc::CDebugProc(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CDebugProc::~CDebugProc(void)
{
}

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT CDebugProc::Init(void)
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 *pDevice = CRenderer::GetDevice();

	// フォント作成
	hr = D3DXCreateFont(*pDevice,
						18,
						0,
						0,
						0,
						FALSE,
						SHIFTJIS_CHARSET,
						OUT_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH,
						"Terminal",
						&m_pD3DXFONT);

	// バッファの初期化
	ZeroMemory(&m_aStr, sizeof(char));

	return hr;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CDebugProc::Uninit(void)
{
	if(m_pD3DXFONT)
	{
		m_pD3DXFONT->Release();
		m_pD3DXFONT = NULL;
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CDebugProc::Draw(void)
{
#ifdef _DEBUG

	// 描画範囲設定
	RECT rect = DRAW_RECT;

	// 描画
	m_pD3DXFONT->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, DRAW_TEXT_COLOR);

	// バッファ初期化
	ZeroMemory(&m_aStr[0],sizeof(char));

	// 文字数初期化
	m_nCounter = 0;
#endif
}

//*****************************************************************************
// 登録処理
//*****************************************************************************
void CDebugProc::Print(const char *fmt, ...)
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = {"\0"};

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start(args, fmt);

	int i;
	for(i = m_nCounter; i < LENGTH_STRING_BUFF; i++)
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if(str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF)
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf(str, fmt, args);

	va_end( args );

	// 文字列合成
	strcat(m_aStr,str);
#endif
}
//----EOF----