//=============================================================================
//
// CUiManagerクラス [CUiManager.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCountTime.h"
#include "CCrowdBar.h"
#include "CHpBar.h"
#include "CStaminaBar.h"
#include "CUiManager.h"
#include "CFace.h"
#include "../../../INPUT/CInputKeyboard.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 顔左サイドの座標
static const D3DXVECTOR2 FACE_POS_LEFT = D3DXVECTOR2(50, 50);
// 顔右サイドの座標
static const D3DXVECTOR2 FACE_POS_RIGHT = D3DXVECTOR2(SCREEN_WIDTH - 50, 50);
// 顔左サイドの大きさ
static const D3DXVECTOR2 FACE_SIZE_LEFT = D3DXVECTOR2(50, 50);
// 顔右サイドの大きさ
static const D3DXVECTOR2 FACE_SIZE_RIGHT = D3DXVECTOR2(50, 50);

// 歓声ゲージの高さ
static const float CROWD_HEIGHT = 30;

// HPゲージの高さと幅
static const float HP_WIDTH = 300;
static const float HP_HEIGHT = 30;
// HPゲージの中心からの距離
static const float HP_CENTER_DIST_X = 30;

//=============================================================================
// コンストラクタ
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_pStaminaBarL = NULL;
	m_pStaminaBarR = NULL;
	m_pCrowdBar = NULL;
	m_pHpBarL = NULL;
	m_pHpBarR = NULL;
	m_pFace = NULL;
	m_pTimer = NULL;
	m_isAnimation = false;
	m_AnimationCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUiManager::~CUiManager(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CUiManager::Init()
{
	// いまコレ描画してない
	m_pStaminaBarL = CStaminaBar::Create(
		D3DXVECTOR2(50, 100),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f - 50, 100),
		CStaminaBar::POSITIONBASE_LEFT, m_pDevice);
	m_pStaminaBarR = CStaminaBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f + 50, 100),
		D3DXVECTOR2(SCREEN_WIDTH - 50, 100),
		CStaminaBar::POSITIONBASE_RIGHT, m_pDevice);
	
	// HP
	m_pHpBarL = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_LEFT,
		m_pDevice);
	m_pHpBarR = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_RIGHT,
		m_pDevice);
	
	// 観声
	m_pCrowdBar = CCrowdBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 50),
		CROWD_HEIGHT,
		m_pDevice);
	
	// 顔
	m_pFace = CFace::Create(
		D3DXVECTOR2(FACE_POS_LEFT), D3DXVECTOR2(FACE_SIZE_LEFT),
		D3DXVECTOR2(FACE_POS_RIGHT), D3DXVECTOR2(FACE_SIZE_RIGHT),
		m_pDevice);
	
	// 制限時間の表示
	m_pTimer = CCountTime::Create(D3DXVECTOR2(SCREEN_WIDTH*0.5f, 100), 99, m_pDevice);

}

//=============================================================================
// 終了
//=============================================================================
void CUiManager::Uninit(void)
{
	// こいつらはインスタンスをもってるだけだからdeleteが必要
	m_pCrowdBar->Uninit();
	m_pTimer->Uninit();
	m_pFace->Uninit();

	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pCrowdBar);
	SAFE_DELETE(m_pFace);
}

//=============================================================================
// 更新
//=============================================================================
void CUiManager::Update(void)
{
	// 更新
	m_pCrowdBar->Update();
	m_pTimer->Update();
	m_pFace->Update();
	
	// test
	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))
	{
		m_pStaminaBarL->UseStamina(50);
		m_pStaminaBarR->UseStamina(50);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
		m_pCrowdBar->Add(5);
	if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))
		m_pCrowdBar->Add(-5);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_UP))
	{
		if (m_pHpBarL)	m_pHpBarL->Add(20);
		if (m_pHpBarR)	m_pHpBarR->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
	{
		if (m_pHpBarL)	m_pHpBarL->Add(-20);
		if (m_pHpBarR)	m_pHpBarR->Add(-20);
	}
	
	// アニメーションしてるとき
	if (m_isAnimation){
		m_pHpBarR->Add(2);
		m_pHpBarL->Add(2);
		m_pCrowdBar->Replace(2);
	
		m_AnimationCount++;
		// アニメーション終了
		if (m_AnimationCount > 120){
			m_isAnimation = false;
		}
	}
}

//=============================================================================
// 作成
//=============================================================================
CUiManager* CUiManager::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	CUiManager* p = new CUiManager(pDevice);
	p->Init();
	return p;
}

//=============================================================================
// ゲーム開始のアニメーションをする関数
//=============================================================================
void CUiManager::StartAnimation(void)
{
	// バーを消す
	m_pHpBarL->Reset();
	m_pHpBarR->Reset();
	m_pCrowdBar->Reset();

	m_isAnimation = true;
	m_AnimationCount = 0;
}

//----EOF----