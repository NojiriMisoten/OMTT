//=============================================================================
//
// メイン処理 [main.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// メモリリークチェック用
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#endif
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)    // 警告対策用
#include "dinput.h"
#include "xaudio2.h"
#ifdef _DEBUG
#include "../DEBUG_PROC/CDebugProc.h"
#endif

// メモリリークチェック用
#ifdef _DEBUG
#define   new                   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define   malloc(s)             _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   calloc(c, s)          _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   realloc(p, s)         _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _recalloc(p, c, s)    _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _expand(p, s)         _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// 2D用
#define	FVF_VF	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 3D用
static const int SCREEN_WIDTH = 1280;												// ウインドウの幅
static const int SCREEN_HEIGHT = 720;												// ウインドウの高さ
static const int TARGET_FPS = 60;

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;	// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;	// 反射光
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

typedef struct VertexFormat_tag {
	D3DXVECTOR3 vtx;
	D3DXVECTOR3 nor;
	D3DCOLOR	diffuse;
	D3DXVECTOR2	tex;
}VF;

D3DVERTEXELEMENT9* GetVertexElement(void);

#endif
//----EOF----