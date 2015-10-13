//=============================================================================
//
// CWinDraw�N���X [CWinDraw.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CWinDraw.h"
#include "../ANIMATION/CAnimation.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ���S�����ʒu
static const D3DXVECTOR3 WIN_DRAW_LOGO_CREATE_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0);
static const D3DXVECTOR3 WIN_DRAW_LOGO_APPEAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 0, 0);

// �������S�֘A
static const float WIN_LOGO_WIDTH = 750;
static const float WIN_LOGO_HEIGHT = 150;
static const TEXTURE_TYPE WIN_LOGO_TEXTURE = TEXTURE_WIN_LOGO;
static const int WIN_LOGO_TEXTURE_SEPALATE_X = 1;
static const int WIN_LOGO_TEXTURE_SEPALATE_Y = 1;

// �����������S�֘A
static const float DRAW_LOGO_WIDTH = 600;
static const float DRAW_LOGO_HEIGHT = 150;
static const TEXTURE_TYPE DRAW_LOGO_TEXTURE = TEXTURE_DRAW_LOGO;
static const int DRAW_LOGO_TEXTURE_SEPALATE_X = 1;
static const int DRAW_LOGO_TEXTURE_SEPALATE_Y = 1;


static const float WIN_DRAW_LOGO_FALL_POSY = SCREEN_HEIGHT * 0.5f;
static const float WIN_DRAW_LOGO_FADE_SPEED = 0.03f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CWinDraw::CWinDraw(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_pLogo = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CWinDraw ::~CWinDraw(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CWinDraw::Init()
{
	m_nCount = 0;
	m_bWin = false;
	m_bDraw = false;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CWinDraw::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CWinDraw::Update(void){

	// �����X�V
	if (m_bWin){
		m_nCount++;
	}

	// ���������X�V
	else if (m_bDraw){
		m_nCount++;
	}

}

//*****************************************************************************
// �������S��\��
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
// �����������S��\��
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