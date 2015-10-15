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
const D3DXVECTOR2 FACE_POS_LEFT = D3DXVECTOR2(50, 50);
// 顔右サイドの座標
const D3DXVECTOR2 FACE_POS_RIGHT = D3DXVECTOR2(SCREEN_WIDTH - 50, 50);
// 顔左サイドの大きさ
const D3DXVECTOR2 FACE_SIZE_LEFT = D3DXVECTOR2(50, 50);
// 顔右サイドの大きさ
const D3DXVECTOR2 FACE_SIZE_RIGHT = D3DXVECTOR2(50, 50);

// 歓声ゲージの高さ
const float CROWD_HEIGHT = 30;

// HPゲージの高さと幅
const float HP_WIDTH = 300;
const float HP_HEIGHT = 30;
// HPゲージの中心からの距離
const float HP_CENTER_DIST_X = 30;

//=============================================================================
// コンストラクタ
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_StaminaBarL = nullptr;
	m_StaminaBarR = nullptr;
	m_CrowdBar = nullptr;
	m_HpBarL = nullptr;
	m_HpBarR = nullptr;
	m_Face = nullptr;
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
	// 変数初期化
	m_isAnimation = 0;
	m_AnimationCount = 0;

	// いまコレ描画してない
	m_StaminaBarL = CStaminaBar::Create(
		D3DXVECTOR2(50, 100),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f - 50, 100),
		CStaminaBar::POSITIONBASE_LEFT, m_pDevice);
	m_StaminaBarR = CStaminaBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f + 50, 100),
		D3DXVECTOR2(SCREEN_WIDTH - 50, 100),
		CStaminaBar::POSITIONBASE_RIGHT, m_pDevice);

	// HP
	m_HpBarL = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_LEFT,
		m_pDevice);
	m_HpBarR = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 100),
		HP_WIDTH,
		HP_HEIGHT,
		HP_CENTER_DIST_X,
		CHpBar::POSITIONBASE_RIGHT,
		m_pDevice);

	// 観声
	m_CrowdBar = CCrowdBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 50),
		CROWD_HEIGHT,
		m_pDevice);

	// 顔
	m_Face = CFace::Create(
		FACE_POS_LEFT, FACE_SIZE_LEFT,
		FACE_POS_RIGHT, FACE_SIZE_RIGHT,
		m_pDevice);

	m_Timer = CCountTime::Create(D3DXVECTOR2(SCREEN_WIDTH*0.5f, 100), 99, m_pDevice);

}

//=============================================================================
// 終了
//=============================================================================
void CUiManager::Uninit(void)
{
	// こいつらはインスタンスをもってるだけだからdeleteが必要
	SAFE_DELETE(m_Timer);
	SAFE_DELETE(m_CrowdBar);
	SAFE_DELETE(m_Face);
}

//=============================================================================
// 更新
//=============================================================================
void CUiManager::Update(void)
{
	// 更新
	m_CrowdBar->Update();
	m_Timer->Update();
	m_Face->Update();

	// test
	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))
	{
		m_StaminaBarL->UseStamina(50);
		m_StaminaBarR->UseStamina(50);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
		m_CrowdBar->Add(5);
	if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))
		m_CrowdBar->Add(-5);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_UP))
	{
		if (m_HpBarL)	m_HpBarL->Add(20);
		if (m_HpBarR)	m_HpBarR->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
	{
		if (m_HpBarL)	m_HpBarL->Add(-20);
		if (m_HpBarR)	m_HpBarR->Add(-20);
	}

	// アニメーションしてるとき
	if (m_isAnimation){
		m_HpBarR->Add(2);
		m_HpBarL->Add(2);
		m_CrowdBar->Replace(2);

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
	m_HpBarL->Reset();
	m_HpBarR->Reset();
	m_CrowdBar->Reset();

	m_isAnimation = true;
	m_AnimationCount = 0;
}

//----EOF----