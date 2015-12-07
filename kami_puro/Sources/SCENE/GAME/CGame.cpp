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
#include "FIELD/CFieldManager.h"
#include "FIELD/CROWD/CCrowdManager.h"
#include "JUDGE/CJudgeManager.h"
#include "../../STAGE_DIRECTOR/CDirectorManager.h"
#include "UI/CUiManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 99 * 60;		// 時間 * FPS
static const int INTORO_ANIMATION_FRAME = 60 * 3;
static const D3DXVECTOR3 DEFAULT_LIGHT_POS(0.0f, 750.0f, -450.0f);

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGame ::CGame(void)
{
	m_pUiManager = NULL;
	m_pFieldManager = NULL;
	m_pCrowdManager = NULL;
	m_pCameraManager = NULL;
	m_pJudgeManager = NULL;
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
	m_pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos(0.0f, 50.0f, -150.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	m_pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// フィールド
	CSceneX* pX = CSceneX::Create(pDevice, D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_RING, m_pManager);
	pX->SetScl(3.0f, 2.0f, 3.0f);

	m_pDirectorManager = m_pManager->GetDirectorManager();
	m_pDirectorManager->Init();

	// 観客
	m_pCrowdManager = m_pCrowdManager->Create(m_pD3DDevice, m_pManager);

	// プレイヤー作成
	m_pManager->GetPlayerManager()->CreatePlayer( pDevice, D3DXVECTOR3( -50, 0, 0 ), SKIN_MESH_TYPE_PLAYER );

	// UI作
//	m_pUiManager = CUiManager::Create(pDevice, m_pManager, this);
	m_pUiManager = m_pManager->GetUiManager();
	m_pUiManager->Init( this );

	// ジャッジの作成&初期化
	m_pJudgeManager = m_pManager->GetJudgeManager();
	m_pJudgeManager->Init( m_pManager );
	m_pJudgeManager->SetBattleMode( BATTLE_MOVE );

	m_pFieldManager = CFieldManager::Create(pDevice, m_pManager);

	// ゲームモード
	m_Mode = GAME_INTRO;
	m_PrevMode = GAME_MAX;

	// ライトの位置決定(影はこれ基準)
	D3DXVECTOR3 lighPos = DEFAULT_LIGHT_POS;
	m_pManager->GetCameraManager()->SetLightCamera(m_pD3DDevice, lighPos);

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

	m_pUiManager->Uninit();
	m_pUiManager = NULL;

	m_pFieldManager->Uninit();
	SAFE_DELETE(m_pFieldManager);

	m_pCrowdManager->Uninit();
	SAFE_DELETE(m_pCrowdManager);

	m_pJudgeManager->Uninit();

	m_pDirectorManager->Uninit();

	m_pCameraManager->Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	m_pManager->GetCameraManager()->Update();
	m_pFieldManager->Update();
	m_pManager->GetLightManager()->Update();
	m_pCrowdManager->Update();

	// 現モードの初期化処理&前モードの終了処理
	if (m_PrevMode != m_Mode)
	{
		// 前モードの終了処理
		switch (m_PrevMode)
		{
		case GAME_INTRO:
			UninitGameIntro();
			break;

		case GAME_BATTLE:
			UninitGameBattle();
			break;

		case GAME_FINISH:
			UninitGameFinish();
			break;
		}

		// 現モードの初期化処理
		switch (m_Mode)
		{
		case GAME_INTRO:
			InitGameIntro();
			break;

		case GAME_BATTLE:
			InitGameBattle();
			break;

		case GAME_FINISH:
			InitGameFinish();
			break;
		}
	}
	m_PrevMode = m_Mode;

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
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_FORCE_BATTLE_MODE ) )
	{
		m_pManager->GetPlayerManager()->SetPos( PLAYER_1, D3DXVECTOR3( -25.0f, 0.0f, 0.0f ) );
		m_pManager->GetPlayerManager()->SetPos( PLAYER_2, D3DXVECTOR3( +25.0f, 0.0f, 0.0f ) );

		m_pManager->GetJudgeManager()->SetBattleMode( BATTLE_FIGHT );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_FORCE_MOVE_MODE ) )
	{
		m_pManager->GetPlayerManager()->SetPos( PLAYER_1, D3DXVECTOR3( -50.0f, 0.0f, 0.0f ) );
		m_pManager->GetPlayerManager()->SetPos( PLAYER_2, D3DXVECTOR3( +50.0f, 0.0f, 0.0f ) );
		
		m_pManager->GetJudgeManager()->SetBattleMode( BATTLE_MOVE );
	}


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
	// ジャッジの更新処理
	m_pJudgeManager->Update();

	// ディレクターの更新処理			よく考えたらこれはここだとダメな気がするけどまぁとりあえず
	m_pDirectorManager->Update();

	// UIの更新
	m_pUiManager->Update();

#ifdef _DEBUG
	CDebugProc::PrintR("[GAME]\n");
	CDebugProc::PrintR("Timer:%d\n", (int)(m_BattleTimer / TARGET_FPS));
	CDebugProc::PrintR("GameMode:");
	switch (m_Mode)
	{
		m_BattleTimer = DEFAULT_BATTLE_TIMER;
	case GAME_INTRO:
		CDebugProc::PrintR("GAME_INTRO");
		break;

	case GAME_BATTLE:
		CDebugProc::PrintR("GAME_BATTLE");
		break;

	case GAME_FINISH:
		CDebugProc::PrintR("GAME_FINISH");
		break;
	}
	CDebugProc::PrintR("\n");
	CDebugProc::PrintR("BattleMode:");
#endif

	switch (m_pJudgeManager->GetBattleMode())
	{
		// 移動モード
	case BATTLE_MOVE:
		CDebugProc::PrintR("BATTLE_MOVE");
		break;


		// 戦闘モード
	case BATTLE_FIGHT:
		CDebugProc::PrintR("BATTLE_FIGHT");
		break;
	}

#ifdef _DEBUG
	CDebugProc::PrintR("\n\n");

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
// バトル前初期化
//*****************************************************************************
void CGame::InitGameIntro(void)
{

}

//*****************************************************************************
// バトル中初期化
//*****************************************************************************
void CGame::InitGameBattle(void)
{
	m_BattleTimer = DEFAULT_BATTLE_TIMER;

	m_pCameraManager->CameraSetToCoord(
		D3DXVECTOR3(0.0f, 150.0f, -250.0f),
		D3DXVECTOR3(0.0f, 25.0f, 0.0f));
}

//*****************************************************************************
// バトル終了初期化
//*****************************************************************************
void CGame::InitGameFinish(void)
{

}

//*****************************************************************************
// バトル前終了処理
//*****************************************************************************
void CGame::UninitGameIntro(void)
{

}

//*****************************************************************************
// バトル中終了処理
//*****************************************************************************
void CGame::UninitGameBattle(void)
{

}

//*****************************************************************************
// バトル終了終了処理
//*****************************************************************************
void CGame::UninitGameFinish(void)
{

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