//=============================================================================
//
// CGameクラス [CGame.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CGame.h"
#include "../../MANAGER/CManager.h"
#include "../../TEXTURE/CTexture.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../LIGHT/CLightManager.h"
#include "PLAYER/CPlayerManager.h"


#include "../GAME/UI/CStaminaBar.h"
#include "../GAME/UI/CCountTime.h"
#include "../GAME/UI/CCrowdBar.h"
#include "../GAME/UI/CHpBar.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGame ::CGame(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGame ::~CGame(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CGame::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(TEXTURE_NULL);

	// カメラ作成
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos(0.0f, 100.0f, -250.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	pCameraManager->CreateCamera(cameraPos, cameraPosR);

	CPlayerManager::CreatePlayer(pDevice, D3DXVECTOR3(0, 0, 0), SKIN_MESH_TYPE_TEST);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);


	m_StaminaBarL = CStaminaBar::Create(
		D3DXVECTOR2(50, 70),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f - 50, 70),
		CStaminaBar::POSITIONBASE_LEFT, pDevice);

	m_StaminaBarR = CStaminaBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f + 50, 70),
		D3DXVECTOR2(SCREEN_WIDTH - 50, 70),
		CStaminaBar::POSITIONBASE_RIGHT, pDevice);

	m_CrowdBar = CCrowdBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 120),
		pDevice);

	m_HpBarL = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f-200, 160),
		CHpBar::POSITIONBASE_LEFT,
		pDevice);
	m_HpBarR = CHpBar::Create(
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f+200, 160),
		CHpBar::POSITIONBASE_RIGHT,
		pDevice);

	m_Timer = CCountTime::Create(D3DXVECTOR2(SCREEN_WIDTH*0.5f, 50), 99, pDevice);

	// 音再生
	//CManager::PlaySoundA(SOUND_LABEL_BGM001);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGame::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();
	m_Timer->Uninit();
	delete m_Timer;
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);

	}
	m_Timer->Update();

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

	if (CInputKeyboard::GetKeyboardTrigger(DIK_UP)){
		m_HpBarL->Add(20);
		m_HpBarR->Add(20);
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN)){
		m_HpBarL->Add(-20);
		m_HpBarR->Add(-20);
	}

	
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CGame* CGame::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CGame* pGame = NULL;

	// 作成
	pGame = new CGame;

	// 初期化
	pGame->Init(mode, pDevice);

	return pGame;
}
//----EOF-------