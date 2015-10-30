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
#include "../GAME/UI/CUiManager.h"
#include "PLAYER/CPlayerManager.h"
#include "../../EFECT/CEffect.h"
#include "../../BASE_OBJECT/CSceneX.h"
#include "../../BASE_OBJECT/CScene3D.h"
#include "COMMANDCHART/CCommandChartManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 99 * 60;		// 時間 * FPS
static const int INTORO_ANIMATION_FRAME = 60 * 3;


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//*****************************************************************************
// UIにコマンドチャートマネージャが追加されたら
// 消してください
//*****************************************************************************
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
CCommandChartManager* g_pCommandChartManager;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGame ::CGame(void)
{
	m_pUiManager = NULL;
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
	m_pFade->Init(TEXTURE_DEFAULT);

	// カメラ作成
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos(0.0f, 50.0f, -150.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// プレイヤー作成
	m_pManager->GetPlayerManager()->CreatePlayer(pDevice, D3DXVECTOR3(-50, 0, 0), SKIN_MESH_TYPE_TEST);

	// ******TEST*****
	CSceneX* pX = CSceneX::Create(pDevice, D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_RING, m_pManager);
	pX->SetScl(1.5f, 1.5f, 1.5f);
	//****************

	// UI作成
	m_pUiManager = CUiManager::Create(pDevice, m_pManager, this);

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*****************************************************************************
	// UIにコマンドチャートマネージャが追加されたら
	// 消してください
	//*****************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// コマンドチャートマネージャーの作成
	g_pCommandChartManager = CCommandChartManager::Create(pDevice);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*****************************************************************************
	// UIにコマンドチャートマネージャが追加されたら
	// 消してください
	//*****************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



	// ゲームモード
	m_Mode = GAME_INTRO;
	m_BattleMode = (BATTLE_MODE)-1;

	CEffect *pEffect;
	pEffect = CEffect::Create(30, (char*)L"../data/EFECT/shock_weve001test.efk", true);
	pEffect->Play(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(30, 30, 30));

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

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

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*****************************************************************************
	// UIにコマンドチャートマネージャが追加されたら
	// 消してください
	//*****************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// コマンドチャートマージャの終了処理
	g_pCommandChartManager->Uninit();
	SAFE_DELETE(g_pCommandChartManager);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*****************************************************************************
	// UIにコマンドチャートマネージャが追加されたら
	// 消してください
	//*****************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	m_pUiManager->Uninit();
	SAFE_DELETE(m_pUiManager);
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	m_pManager->GetCameraManager()->Update();
	// 現モードの実行
	switch (m_Mode)
	{
	case GAME_INTRO:
		GameIntro();
		break;

	case GAME_BATTLE:
		GameBattle();
		break;

	case GAME_FINISH:
		GameFinish();
		break;
	}

	// test
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_START_TEST))
	{
		m_pUiManager->StartAnimation(INTORO_ANIMATION_FRAME);
	}

	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_DECIDE))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);
	}
}

//*****************************************************************************
// バトル前
//*****************************************************************************
void CGame::GameIntro(void)
{
	// とりあえずGAME_BATTLEに以降
	m_Mode = GAME_BATTLE;

	// 開始アニメーションの開始
	m_pUiManager->StartAnimation(INTORO_ANIMATION_FRAME);

}

//*****************************************************************************
// バトル中
//*****************************************************************************
void CGame::GameBattle(void)
{
	// GameBattle初期化
	if (m_BattleMode == -1)
	{
		m_BattleTimer = DEFAULT_BATTLE_TIMER;
		m_BattleMode = BATTLE_MOVE;
	}

	// BattleModeチェック（移動か攻撃か）
	switch (m_BattleMode)
	{
		// 移動モード
	case BATTLE_MOVE:
		// プレイヤー同士が近づいたら
		if (0)
		{
			// 戦闘モード移行
			m_BattleMode = BATTLE_FIGHT;
		}
		break;


		// 戦闘モード
	case BATTLE_FIGHT:
		// プレイヤー同士が離れたら
		if (0)
		{
			// 移動モード移行
			m_BattleMode = BATTLE_MOVE;
		}
		break;
	}

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*****************************************************************************
	// UIにコマンドチャートマネージャが追加されたら
	// 消してください
	//*****************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// コマンドチャートの更新
	g_pCommandChartManager->Update();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*****************************************************************************
	// UIにコマンドチャートマネージャが追加されたら
	// 消してください
	//*****************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// UIの更新
	m_pUiManager->Update();

#ifdef _DEBUG
	CDebugProc::PrintL("[GAME]\n");
	CDebugProc::PrintL("Timer:%d\n", (int)(m_BattleTimer / TARGET_FPS));
	CDebugProc::PrintL("\n");
#endif

	// 時間減少
	m_BattleTimer--;

	// バトル終了条件
	// 時間制限
	if (m_BattleTimer <= 0)
	{
	//	m_Mode = GAME_FINISH;
	}

	// 体力0
	if (0)
	{
		m_Mode = GAME_FINISH;
	}
}

//*****************************************************************************
// バトル終了
//*****************************************************************************
void CGame::GameFinish(void)
{
	// とりあえず終了
	// フェードアウト開始
	m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

	// リザルトへ
	m_pManager->SetNextPhase(MODE_PHASE_RESULT);
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