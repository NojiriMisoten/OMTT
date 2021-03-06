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
#include "CUiManager.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "../CGame.h"
#include "../COMMANDCHART/CCommandChartManager.h"
#include "CCutIn.h"
#include "CBattleFade.h"
#include "../../../MANAGER/CManager.h"
#include "COverLay.h"
#include "CRopeTimer.h"
#include "CForcusLine.h"
#include "CJumpEffectBillbord.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 歓声ゲージの高さ
static const float CROWD_HEIGHT = SCREEN_HEIGHT / 720.f * 15.f;
// 歓声ゲージのY座標
static const float CROWD_POS_Y = SCREEN_HEIGHT / 720.f * 75.f;
// 歓声ゲージの左の座標
static const float CROWD_POS_LEFT_X = SCREEN_WIDTH / 1280.f * 225.f;
// 歓声ゲージの右の座標
static const float CROWD_POS_RIGHT_X = SCREEN_WIDTH - (SCREEN_WIDTH / 1280.f * 225.f);

// HPゲージの高さと幅
static const float HP_HEIGHT = SCREEN_HEIGHT / 720.f * 34.f;
// HPゲージのY座標
static const float HP_POS_Y = SCREEN_HEIGHT / 720.f * 121.f;
// HPゲージの左サイドのX座標 得点に近いほうがSTART
static const float HP_POS_X_LEFT_START = SCREEN_WIDTH * 0.5f - SCREEN_WIDTH / 1280.f * 50.f;
static const float HP_POS_X_LEFT_END = SCREEN_WIDTH / 1280.f * 120.f;
// HPゲージの右サイドのX座標 得点に近いほうがSTART
static const float HP_POS_X_RIGHT_START = SCREEN_WIDTH * 0.5f + (SCREEN_WIDTH / 1280.f * 50.f);
static const float HP_POS_X_RIGHT_END = SCREEN_WIDTH - (SCREEN_WIDTH / 1280.f * 120.f);

// ゲームのカウントタイム
static const int COUNT_TIMER_MAX = 99;
// ゲームのカウントタイムの座標
static const D3DXVECTOR2 COUNT_TIME_POS = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT / 720.f * 120.f);

// 燃やし始めるHP
static const float FIRE_HP = DEFAULT_HP_PARAMETER * 0.3f;
static const float FIRE_CROWD = CROWD_MAX * 0.7f;

//=============================================================================
// コンストラクタ
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CUiManager::CUiManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CCameraManager* pCamera)
{
	m_pDevice = pDevice;
	m_pStaminaBarL = NULL;
	m_pStaminaBarR = NULL;
	m_pCrowdBar = NULL;
	m_pHpBar = NULL;
	m_pTimer = NULL;
	m_pManager = pManager;
	m_pCommandChartManager = NULL;
	m_pCamera = pCamera;
	m_pCutIn = NULL;
	m_pBattleFade = NULL;
	m_pRopeTimer = NULL;
	m_pForcusLine = NULL;
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
void CUiManager::Init(CGame *pGame)
{
	m_pGame = pGame;
	
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
	
	// 制限時間の表示
	m_pTimer = CCountTime::Create(
		D3DXVECTOR2(COUNT_TIME_POS),
		COUNT_TIMER_MAX,
		m_pDevice,
		m_pManager,
		m_pGame);

	// 集中線
	m_pForcusLine = CForcusLine::Create(m_pDevice);
	// コマンドチャートマネージャーの作成
	m_pCommandChartManager = CCommandChartManager::Create(m_pDevice, m_pManager->GetPlayerManager());
	// カットイン
	m_pCutIn = CCutIn::Create(m_pDevice);
	// オーバーレイ
	m_pOverLay = COverLay::Create(m_pDevice);
	// ロープタイマー
	m_pRopeTimer = CRopeTimer::Create(m_pDevice);
	// バトルフェード
	m_pBattleFade = CBattleFade::Create(m_pDevice);
}

//=============================================================================
// 終了
//=============================================================================
void CUiManager::Uninit(void)
{
	// こいつらはインスタンスをもってるだけだからdeleteが必要
	m_pForcusLine->Uninit();
	m_pRopeTimer->Uninit();
	m_pOverLay->Uninit();
	m_pBattleFade->Uninit();
	m_pCutIn->Uninit();
	m_pCrowdBar->Uninit();
	m_pTimer->Uninit();
	m_pHpBar->Uninit();
	m_pCommandChartManager->Uninit();

	SAFE_DELETE(m_pForcusLine);
	SAFE_DELETE(m_pRopeTimer);
	SAFE_DELETE(m_pOverLay);
	SAFE_DELETE(m_pBattleFade);
	SAFE_DELETE(m_pCutIn);
	SAFE_DELETE(m_pCrowdBar);
	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pHpBar);
	SAFE_DELETE(m_pCommandChartManager);
}

//=============================================================================
// 更新
//=============================================================================
void CUiManager::Update(void)
{
	// 各UIの更新
	m_pRopeTimer->Update();
	m_pOverLay->Update();
	m_pCrowdBar->Update();
	m_pTimer->Update();
	m_pHpBar->Update();
	m_pCutIn->Update();
	m_pBattleFade->Update();
	m_pForcusLine->Update();
	// コマンドチャートの更新
	m_pCommandChartManager->Update();

	// じじいを燃やす管理
	UpdateFireOldMan();

#ifdef _DEBUG
	// 観客ゲージの増減
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_RIGHT_TEST))
	{
		m_pCrowdBar->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_UP_CROWD_LEFT_TEST))
	{
		m_pCrowdBar->Add(-20);
	}
	// HPの増減
	if (CInputKeyboard::GetKeyboardRepeat(KEYBOARD_CODE_UI_UP_HP_TEST))
	{
		m_pHpBar->AddLeft(5);
		m_pHpBar->AddRight(5);
	}
	if (CInputKeyboard::GetKeyboardRepeat(KEYBOARD_CODE_UI_DOWN_HP_TEST))
	{
		m_pHpBar->SubLeft(5);
		m_pHpBar->SubRight(5);
	}
	// カットインのテスト
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_CUT_IN_0))
	{
		m_pCutIn->Start(0, CUT_IN_JIJII);
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_CUT_IN_1))
	{
		m_pCutIn->Start(1, CUT_IN_JIJII);
	}
	// シーン切り替えとかのフェード
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_FADE))
	{
		m_pBattleFade->Start(BATTLE_FADE_LIGHT);
	}
	// オーバーレイ(Readyとか)
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_OVERLAY))
	{
		m_pOverLay->Start(&COverLay::Data(TEXTURE_ROPE, 0.1f, 30, 0.1f));
	}
	// ロープタイマー
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_ROPE_TIMER))
	{
		m_pRopeTimer->Start(30 ,80);
	}
	// 時計を止める
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_STOP_TIMER))
	{
		m_pTimer->ChainAnimeStart();
		m_pHpBar->FireLeft(true);
	}
	// 集中線だす
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_FORCUS_LINE))
	{
		m_pTimer->ChainAnimeStop();
		m_pForcusLine->Start(60, false);
		m_pHpBar->FireLeft(false);
	}
#endif
}
//=============================================================================
// じじいを燃やす管理の更新
//=============================================================================
void CUiManager::UpdateFireOldMan(void)
{
	// HPと観客ゲージ取得
	float hpL = m_pHpBar->GetHPLeft();
	float hpR = m_pHpBar->GetHPRight();
	float crowd = m_pCrowdBar->GetAmount();
	// ジャッジ！
	bool isBurnL = (hpR < FIRE_HP) | (crowd > FIRE_CROWD);
	bool isBurnR = (hpL < FIRE_HP) | (crowd < -FIRE_CROWD);
	// 燃やすときは燃やす
	m_pHpBar->FireLeft(isBurnL);
	m_pHpBar->FireRight(isBurnR);
}

/*
//=============================================================================
// 作成
//=============================================================================
CUiManager* CUiManager::Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame *pGame)
{
	CUiManager* p = new CUiManager(pDevice, pManager);
	p->Init(pGame);
	return p;
}
*/
//=============================================================================
// ゲーム開始のアニメーションをする関数
//=============================================================================
void CUiManager::StartAnimation(int interval)
{
	// 各UIアニメーションを開始
	m_pTimer->StartAnimation(interval);
	m_pHpBar->StartAnimation(interval);
	m_pCrowdBar->StartAnimation(interval);
}

//=============================================================================
// バトルフェードスタート
//=============================================================================
void CUiManager::StartBattleFade(void)
{ 
	m_pBattleFade->Start(BATTLE_FADE_LIGHT); 
	m_pManager->PlaySoundA(SOUND_LABEL_SE_BATTLE_FADE);
}

//=============================================================================
// 集中線を出す
//=============================================================================
void CUiManager::StartForcusLine(int timerFrameCount, bool isFinish)
{
	m_pForcusLine->Start(timerFrameCount, isFinish);
}

//=============================================================================
// オーバーレイスタート
//=============================================================================
void CUiManager::StartOverLay(COverLay::Data *data)
{
	m_pOverLay->Start(data);
}

//=============================================================================
// 表示
//=============================================================================
void CUiManager::SetImvisible(void)
{
	m_pHpBar->SetImvisible();
	m_pCrowdBar->SetImvisible();
	m_pTimer->SetImvisible();
}

//=============================================================================
// 非表示
//=============================================================================
void CUiManager::SetVisible(void)
{
	m_pCrowdBar->SetVisible();
	m_pTimer->SetVisible();
	m_pHpBar->SetVisible();
}

//=============================================================================
// プレイヤがジャンプしたときに出すエフェクト
//=============================================================================
void CUiManager::CreateJumpEffect(D3DXVECTOR3& pos, TEXTURE_TYPE texture, int playerNum)
{
	CJumpEffectBillbord *a = CJumpEffectBillbord::Create(
		m_pDevice, pos, texture, m_pCamera, this, playerNum);
}
//=============================================================================
// 歓声ゲージの最小から最大の2D座標
//=============================================================================
float CUiManager::GetPosCrowdCenter(int playerNum)
{
	return m_pCrowdBar->GetPosCrowdCenter(playerNum);
}
//=============================================================================
// HPゲージの最小から最大の2D座標
//=============================================================================
float CUiManager::GetPosHpCenter(int playerNum)
{
	return m_pHpBar->GetPosHpCenter(playerNum);
}
//----EOF----