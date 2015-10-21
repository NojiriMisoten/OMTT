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
//*****************************************************************************
// マクロ
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 99 * 60;		// 時間 * FPS

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

	// ゲームモード
	m_Mode = GAME_INTRO;
	m_BattleMode = (BATTLE_MODE)-1;

	CPlayerManager::CreatePlayer(pDevice, D3DXVECTOR3(0, 0, 0), SKIN_MESH_TYPE_TEST);

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
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	m_pManager->GetCameraManager()->Update();


	// 現モードの実行
	switch( m_Mode )
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

#ifdef _DEBUG
	CDebugProc::Print( "GameMode:%d\n", m_Mode );
	CDebugProc::Print( "BattleMode:%d\n", m_Mode );
#endif

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// リザルトへ
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);
	}
}

void CGame::GameIntro( void )
{
	// とりあえずGAME_BATTLEに以降
	m_Mode = GAME_BATTLE;
}

void CGame::GameBattle( void )
{
	// GameBattle初期化
	if( m_BattleMode == -1 )
	{
		m_BattleTimer = DEFAULT_BATTLE_TIMER;
		m_BattleMode = BATTLE_MOVE;
	}

	// BattleModeチェック（移動か攻撃か）
	switch( m_BattleMode )
	{
	// 移動モード
	case BATTLE_MOVE:
		// プレイヤー同士が近づいたら
		if( 0 )
		{
			// 戦闘モード移行
			m_BattleMode = BATTLE_FIGHT;
		}
		break;


	// 戦闘モード
	case BATTLE_FIGHT:
		// プレイヤー同士が離れたら
		if( 0 )
		{
			// 移動モード移行
			m_BattleMode = BATTLE_MOVE;
		}
		break;
	}

#ifdef _DEBUG
	CDebugProc::Print( "Timer:%d\n", (int)( m_BattleTimer / 60 ) );
#endif

	// 時間減少
	m_BattleTimer--;

	// バトル終了条件
	// 時間制限
	if( m_BattleTimer <= 0 )
	{
		m_Mode = GAME_FINISH;
	}
	
	// 体力0
	if( 0 )
	{
		m_Mode = GAME_FINISH;
	}
}

void CGame::GameFinish( void )
{
	// とりあえず終了
	// フェードアウト開始
	m_pFade->Start( MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME );

	// リザルトへ
	m_pManager->SetNextPhase( MODE_PHASE_RESULT );
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