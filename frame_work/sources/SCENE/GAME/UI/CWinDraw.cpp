//=============================================================================
//
// CWinDrawクラス [CWinDraw.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CWinDraw.h"
#include "../ANIMATION/CAnimation.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ロゴ生成位置
static const D3DXVECTOR3 WIN_DRAW_LOGO_CREATE_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0);
static const D3DXVECTOR3 WIN_DRAW_LOGO_APPEAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 0, 0);

// 勝ちロゴ関連
static const float WIN_LOGO_WIDTH = 750;
static const float WIN_LOGO_HEIGHT = 150;
static const TEXTURE_TYPE WIN_LOGO_TEXTURE = TEXTURE_WIN_LOGO;
static const int WIN_LOGO_TEXTURE_SEPALATE_X = 1;
static const int WIN_LOGO_TEXTURE_SEPALATE_Y = 1;

// 引き分けロゴ関連
static const float DRAW_LOGO_WIDTH = 600;
static const float DRAW_LOGO_HEIGHT = 150;
static const TEXTURE_TYPE DRAW_LOGO_TEXTURE = TEXTURE_DRAW_LOGO;
static const int DRAW_LOGO_TEXTURE_SEPALATE_X = 1;
static const int DRAW_LOGO_TEXTURE_SEPALATE_Y = 1;


static const float WIN_DRAW_LOGO_FALL_POSY = SCREEN_HEIGHT * 0.5f;
static const float WIN_DRAW_LOGO_FADE_SPEED = 0.03f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CWinDraw::CWinDraw(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_pLogo = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CWinDraw ::~CWinDraw(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CWinDraw::Init()
{
	m_nCount = 0;
	m_bWin = false;
	m_bDraw = false;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CWinDraw::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CWinDraw::Update(void){

	// 勝ち更新
	if (m_bWin){
		m_nCount++;
	}

	// 引き分け更新
	else if (m_bDraw){
		m_nCount++;
	}

}

//*****************************************************************************
// 勝ちロゴを表示
//*****************************************************************************
void CWinDraw::CreateWinLogo(void){

	m_pLogo = CAnimation::Create(
		m_pDevice,
		TYPE_PRIORITY_UI,
		WIN_DRAW_LOGO_CREATE_POS,
		WIN_LOGO_WIDTH,
		WIN_LOGO_HEIGHT,
		WIN_LOGO_TEXTURE,
		WIN_LOGO_TEXTURE_SEPALATE_X,
		WIN_LOGO_TEXTURE_SEPALATE_Y);

}

//*****************************************************************************
// 引き分けロゴを表示
//*****************************************************************************
void CWinDraw::CreateDrawLogo(void){

	m_pLogo = CAnimation::Create(
		m_pDevice,
		TYPE_PRIORITY_UI,
		WIN_DRAW_LOGO_CREATE_POS,
		DRAW_LOGO_WIDTH,
		DRAW_LOGO_HEIGHT,
		DRAW_LOGO_TEXTURE,
		DRAW_LOGO_TEXTURE_SEPALATE_X,
		DRAW_LOGO_TEXTURE_SEPALATE_Y);


}

//----EOF-------