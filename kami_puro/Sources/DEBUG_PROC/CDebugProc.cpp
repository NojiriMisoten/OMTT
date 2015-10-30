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
static const D3DXCOLOR DRAW_TEXT_COLOR = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);	// 文字の色

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
LPD3DXFONT	CDebugProc::m_pD3DXFONT = NULL;						// フォントへのポインタ
char		CDebugProc::m_aStrL[LENGTH_STRING_BUFF] = { '\0' };	// デバッグ表示用の文字列のバッファ
char		CDebugProc::m_aStrR[LENGTH_STRING_BUFF] = { '\0' };
char		CDebugProc::m_aStrLD[LENGTH_STRING_BUFF] = { '\0' };
char		CDebugProc::m_aStrRD[LENGTH_STRING_BUFF] = { '\0' };
char		CDebugProc::m_aStrU[LENGTH_STRING_BUFF] = { '\0' };
char		CDebugProc::m_aStrD[LENGTH_STRING_BUFF] = { '\0' };
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
	ZeroMemory( &m_aStrL, sizeof( char ) );
	ZeroMemory( &m_aStrR, sizeof( char ) );
	ZeroMemory( &m_aStrU, sizeof( char ) );
	ZeroMemory( &m_aStrD, sizeof( char ) );

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
	m_pD3DXFONT->DrawText( NULL, m_aStrL, -1, &rect, DT_LEFT, DRAW_TEXT_COLOR );
	m_pD3DXFONT->DrawText( NULL, m_aStrR, -1, &rect, DT_RIGHT, DRAW_TEXT_COLOR );
	m_pD3DXFONT->DrawText( NULL, m_aStrLD, -1, &rect, DT_LEFT|DT_BOTTOM, DRAW_TEXT_COLOR );
	m_pD3DXFONT->DrawText( NULL, m_aStrRD, -1, &rect, DT_RIGHT|DT_BOTTOM, DRAW_TEXT_COLOR );
	m_pD3DXFONT->DrawText( NULL, m_aStrU, -1, &rect, DT_CENTER, DRAW_TEXT_COLOR );
	m_pD3DXFONT->DrawText( NULL, m_aStrD, -1, &rect, DT_CENTER|DT_BOTTOM, DRAW_TEXT_COLOR );

	// バッファ初期化
	ZeroMemory( &m_aStrL[0], sizeof( char ) );
	ZeroMemory( &m_aStrR[0], sizeof( char ) );
	ZeroMemory( &m_aStrLD[0], sizeof( char ) );
	ZeroMemory( &m_aStrRD[0], sizeof( char ) );
	ZeroMemory( &m_aStrU[0], sizeof( char ) );
	ZeroMemory( &m_aStrD[0], sizeof( char ) );

	// 文字数初期化
	m_nCounter = 0;
#endif
}

//*****************************************************************************
// 登録処理
//*****************************************************************************
void CDebugProc::PrintL( const char *fmt, ... )
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = { "\0" };

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start( args, fmt );

	int i;
	for( i = m_nCounter; i < LENGTH_STRING_BUFF; i++ )
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if( str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF )
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf( str, fmt, args );

	va_end( args );

	// 文字列合成
	strcat( m_aStrL, str );
#endif
}

void CDebugProc::PrintR( const char *fmt, ... )
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = { "\0" };

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start( args, fmt );

	int i;
	for( i = m_nCounter; i < LENGTH_STRING_BUFF; i++ )
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if( str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF )
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf( str, fmt, args );

	va_end( args );

	// 文字列合成
	strcat( m_aStrR, str );
#endif
}

void CDebugProc::PrintU( const char *fmt, ... )
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = { "\0" };

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start( args, fmt );

	int i;
	for( i = m_nCounter; i < LENGTH_STRING_BUFF; i++ )
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if( str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF )
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf( str, fmt, args );

	va_end( args );

	// 文字列合成
	strcat( m_aStrU, str );
#endif
}

void CDebugProc::PrintD( const char *fmt, ... )
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = { "\0" };

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start( args, fmt );

	int i;
	for( i = m_nCounter; i < LENGTH_STRING_BUFF; i++ )
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if( str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF )
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf( str, fmt, args );

	va_end( args );

	// 文字列合成
	strcat( m_aStrD, str );
#endif
}

void CDebugProc::PrintLD( const char *fmt, ... )
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = { "\0" };

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start( args, fmt );

	int i;
	for( i = m_nCounter; i < LENGTH_STRING_BUFF; i++ )
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if( str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF )
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf( str, fmt, args );

	va_end( args );

	// 文字列合成
	strcat( m_aStrLD, str );
#endif
}

void CDebugProc::PrintRD( const char *fmt, ... )
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = { "\0" };

	// 可変引数保存用
	va_list args;

	// 可変引数初期化
	va_start( args, fmt );

	int i;
	for( i = m_nCounter; i < LENGTH_STRING_BUFF; i++ )
	{
		// 文字数カウント
		m_nCounter++;

		// バッファオーバーフロー防止
		if( str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF )
		{
			break;
		}
	}

	// 渡された文字列一時保存
	vsprintf( str, fmt, args );

	va_end( args );

	// 文字列合成
	strcat( m_aStrRD, str );
#endif
}
//----EOF----