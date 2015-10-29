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
static const D3DXVECTOR2 FACE_POS_LEFT = D3DXVECTOR2(90, 140);
// 顔右サイドの座標
static const D3DXVECTOR2 FACE_POS_RIGHT = D3DXVECTOR2(SCREEN_WIDTH - 90, 140);
// 顔の大きさ
static const D3DXVECTOR2 FACE_SIZE = D3DXVECTOR2(100, 100);

// 歓声ゲージの高さ
static const float CROWD_HEIGHT = 20;
// 歓声ゲージのY座標
static const float CROWD_POS_Y = 75;
// 歓声ゲージの左の座標
static const float CROWD_POS_LEFT_X = 170;
// 歓声ゲージの右の座標
static const float CROWD_POS_RIGHT_X = SCREEN_WIDTH - 170;

// HPゲージの高さと幅
static const float HP_HEIGHT = 30;
// HPゲージのY座標
static const float HP_POS_Y = 125;
// HPゲージの左サイドのX座標 得点に近いほうがSTART
static const float HP_POS_X_LEFT_START = SCREEN_WIDTH * 0.5f - 50;
static const float HP_POS_X_LEFT_END = 100;
// HPゲージの右サイドのX座標 得点に近いほうがSTART
static const float HP_POS_X_RIGHT_START = SCREEN_WIDTH * 0.5f + 50;
static const float HP_POS_X_RIGHT_END = SCREEN_WIDTH - 100;

// ゲームのカウントタイム
static const int COUNT_TIMER_MAX = 99;
// ゲームのカウントタイムの座標
static const D3DXVECTOR2 COUNT_TIME_POS = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 125);

//=============================================================================
// コンストラクタ
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
	m_pStaminaBarL = NULL;
	m_pStaminaBarR = NULL;
	m_pCrowdBar = NULL;
	m_pHpBar = NULL;
	m_pFace = NULL;
	m_pTimer = NULL;
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
	m_pHpBar = CHpBar::Create(
		HP_HEIGHT,
		HP_POS_Y,
		HP_POS_X_LEFT_END, HP_POS_X_LEFT_START,
		HP_POS_X_RIGHT_START, HP_POS_X_RIGHT_END,
		m_pDevice);
	
	// 観声
	m_pCrowdBar = CCrowdBar::Create(
		CROWD_HEIGHT, CROWD_POS_Y,
		CROWD_POS_LEFT_X, CROWD_POS_RIGHT_X,
		m_pDevice);
	
	// 顔
	m_pFace = CFace::Create(
		D3DXVECTOR2(FACE_POS_LEFT), D3DXVECTOR2(FACE_POS_RIGHT),
		D3DXVECTOR2(FACE_SIZE), m_pDevice);
	
	// 制限時間の表示
	m_pTimer = CCountTime::Create(D3DXVECTOR2(COUNT_TIME_POS), COUNT_TIMER_MAX, m_pDevice);

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
	m_pHpBar->Uninit();

	SAFE_DELETE(m_pCrowdBar);
	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pFace);
	SAFE_DELETE(m_pHpBar);
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
	m_pHpBar->Update();

	// test
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_USE_STAMINA_TEST))
	{
		m_pStaminaBarL->UseStamina(50);
		m_pStaminaBarR->UseStamina(50);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_RIGHT_TEST))
	{
		m_pCrowdBar->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_LEFT_TEST))
	{
		m_pCrowdBar->Add(-20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_HP_TEST))
	{
		m_pHpBar->AddLeft(20);
		m_pHpBar->AddRight(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_DOWN_HP_TEST))
	{
		m_pHpBar->SubLeft(20);
		m_pHpBar->SubRight(20);
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
void CUiManager::StartAnimation(int interval)
{
	// 各UIアニメーションを開始
	m_pTimer->StartAnimation(interval);
	m_pFace->StartAnimation(interval);
	m_pHpBar->StartAnimation(interval);
	m_pCrowdBar->StartAnimation(interval);
}

//----EOF----