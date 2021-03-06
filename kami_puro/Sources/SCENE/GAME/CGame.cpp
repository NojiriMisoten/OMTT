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
#include "UI/CCrowdBar.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 100 * 60;		// 時間 * FPS
static const int INTORO_ANIMATION_FRAME = 60 * 3;
static const D3DXVECTOR3 DEFAULT_LIGHT_POS(0.0f, 750.0f, -450.0f);
static const D3DXVECTOR3 INIT_CAMERA_POS(-150.f, 400.0f, 0.0f);
static const D3DXVECTOR3 INIT_CAMERA_POSR(0.f, 0.0f, 0.0f);

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
	m_pCrowdBar = NULL;
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
	D3DXVECTOR3	cameraPos = INIT_CAMERA_POS;
	D3DXVECTOR3	cameraPosR = INIT_CAMERA_POSR;
	m_pCameraManager->CreateCamera(cameraPos, cameraPosR);

	m_pDirectorManager = m_pManager->GetDirectorManager();
	m_pDirectorManager->Init( this );

	// 観客
	m_pCrowdManager = m_pCrowdManager->Create(m_pD3DDevice, m_pManager);

	// プレイヤー作成
	m_pManager->GetPlayerManager()->CreatePlayer( pDevice, D3DXVECTOR3( -50, 0, 0 ), SKIN_MESH_TYPE_PLAYER01 );

	// UI作
	m_pUiManager = m_pManager->GetUiManager();
	m_pUiManager->Init( this );

	// ジャッジの作成&初期化
	m_pJudgeManager = m_pManager->GetJudgeManager();
	m_pJudgeManager->Init( m_pManager );
	m_pJudgeManager->SetBattleMode( BATTLE_MAX );

	// フィールドマネージャー作成
	m_pFieldManager = CFieldManager::Create(pDevice, m_pManager);

	// 観客バーポインタ
	m_pCrowdBar = m_pUiManager->GetCrowdBar();

	// ゲームモード
	m_Mode = GAME_INTRO;
	m_PrevMode = GAME_MAX;

	// ライトの位置決定(影はこれ基準)
	D3DXVECTOR3 lighPos = DEFAULT_LIGHT_POS;
	m_pManager->GetCameraManager()->SetLightCamera(m_pD3DDevice, lighPos);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// 音再生
	CManager::PlaySoundA(SOUND_LABEL_BGM_GAME);
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

	// ディレクターの更新処理			よく考えたらこれはここだとダメな気がするけどまぁとりあえず
	m_pDirectorManager->Update();

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

	// 緊急用
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_GO_TITLE))
	{
		m_Mode = GAME_FINISH;
	}

#ifdef _DEBUG
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
		m_pFade->Start( MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME );

		// タイトルヘ
		m_pManager->SetNextPhase( MODE_PHASE_TITLE );
	}
#endif

#ifdef _DEBUG
	CDebugProc::PrintR( "[GAME]\n" );
	CDebugProc::PrintR( "Timer:%d\n", (int)( m_BattleTimer / TARGET_FPS ) );
	CDebugProc::PrintR( "GameMode:" );
	switch( m_Mode )
	{
		m_BattleTimer = DEFAULT_BATTLE_TIMER;
	case GAME_INTRO:
		CDebugProc::PrintR( "GAME_INTRO" );
		break;

	case GAME_BATTLE:
		CDebugProc::PrintR( "GAME_BATTLE" );
		break;

	case GAME_FINISH:
		CDebugProc::PrintR( "GAME_FINISH" );
		break;
	}
	CDebugProc::PrintR( "\n" );
	CDebugProc::PrintR( "BattleMode:" );

	switch( m_pJudgeManager->GetBattleMode() )
	{
		// 移動モード
	case BATTLE_MOVE:
		CDebugProc::PrintR( "BATTLE_MOVE" );
		break;


		// 戦闘モード
	case BATTLE_FIGHT:
		CDebugProc::PrintR( "BATTLE_FIGHT" );
		break;
	}

	CDebugProc::PrintR( "\n\n" );

#endif

}

//*****************************************************************************
// バトル前
//*****************************************************************************
void CGame::GameIntro( void )
{
	// UIの更新
	m_pUiManager->Update();

	if( m_BattleTimer == 180 )
	{
		// 開始アニメーションの開始
		m_pUiManager->StartAnimation( INTORO_ANIMATION_FRAME );
	}
	if( m_BattleTimer < 0 )
	{
		// とりあえずGAME_BATTLEに以降
		m_Mode = GAME_BATTLE;
	}

	// 時間減少（タイミング管理用に流用しているだけ）
	m_BattleTimer--;
}

//*****************************************************************************
// バトル中
//*****************************************************************************
void CGame::GameBattle( void )
{

	// ジャッジの更新処理
	m_pJudgeManager->Update();

	// UIの更新
	m_pUiManager->Update();

	// 必殺条件
	// : 相手のＨＰ３０％以下もしくは歓声ゲージ７０％以上
	if( ( m_pManager->GetPlayerManager()->GetPlayerHP( PLAYER_2 ) < ( DEFAULT_HP_PARAMETER * 0.3f ) )
		|| ( m_pManager->GetUiManager()->GetCrowdBar()->GetAmount() > ( CROWD_MAX * 0.7f ) ) )
	{
		m_pManager->GetPlayerManager()->SetUseFinishFlag( PLAYER_1, true );
	}
	else
	{
		m_pManager->GetPlayerManager()->SetUseFinishFlag( PLAYER_1, false );
	}

	if( ( m_pManager->GetPlayerManager()->GetPlayerHP( PLAYER_1 ) < ( DEFAULT_HP_PARAMETER * 0.3f ) )
		|| ( m_pManager->GetUiManager()->GetCrowdBar()->GetAmount() < ( -CROWD_MAX * 0.7f ) ) )
	{
		m_pManager->GetPlayerManager()->SetUseFinishFlag( PLAYER_2, true );
	}
	else
	{
		m_pManager->GetPlayerManager()->SetUseFinishFlag( PLAYER_2, false );
	}

	// 演出再生中であれば
	if( m_pManager->GetDirectorManager()->GetIsDirecting().directingID != (DIRECT_ID)-1 )
	{
		// タイマー減少しない
	}
	else
	{
		// 時間減少
		m_BattleTimer--;
	}

	// バトル終了条件
	// 時間制限
	if (m_BattleTimer == 0)
	{
		// 強制的にバトルモード
		m_pManager->GetPlayerManager()->SetPos( PLAYER_1, D3DXVECTOR3( -25.0f, 0.0f, 0.0f ) );
		m_pManager->GetPlayerManager()->SetPos( PLAYER_2, D3DXVECTOR3( +25.0f, 0.0f, 0.0f ) );

		m_pManager->GetJudgeManager()->SetBattleMode( BATTLE_FIGHT );

		m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( ( PLAYER_1 ), CCommandChart::MODE_VANISH );
		m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( ( PLAYER_2 ), CCommandChart::MODE_VANISH );

		// プレイヤー１の方が体力が多い
		if( m_pManager->GetPlayerManager()->GetPlayerHP( PLAYER_1 ) > m_pManager->GetPlayerManager()->GetPlayerHP( PLAYER_2 ) )
		{
			// 必殺発動
			m_pManager->GetDirectorManager()->Direct( DIR_FINISHER, PLAYER_1 );
		}
		else
		{
			// 必殺発動
			m_pManager->GetDirectorManager()->Direct( DIR_FINISHER, PLAYER_2 );
		}
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
	m_pManager->SetNextPhase(MODE_PHASE_TITLE);
}

//*****************************************************************************
// バトル前初期化
//*****************************************************************************
void CGame::InitGameIntro(void)
{
	m_BattleTimer = 200;
	m_pManager->GetDirectorManager()->Direct( DIR_BATTLE_INTRO, PLAYER_1 );
}

//*****************************************************************************
// バトル中初期化
//*****************************************************************************
void CGame::InitGameBattle(void)
{
	m_BattleTimer = DEFAULT_BATTLE_TIMER;

	m_pJudgeManager->SetBattleMode( BATTLE_MOVE );

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

//*****************************************************************************
// プレイヤー以外描画しない
//*****************************************************************************
void CGame::SetImvisible( void )
{
	m_pFieldManager->SetImvisible();
	m_pCrowdManager->SetImvisible();
	m_pUiManager->SetImvisible();
}

//*****************************************************************************
// 全て描画する
//*****************************************************************************
void CGame::SetVisible( void )
{
	m_pFieldManager->SetVisible();
	m_pCrowdManager->SetVisible();
	m_pUiManager->SetVisible();
}
//----EOF-------