//=============================================================================
//
// CGameクラス [CGame.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneScore.h"
#include "CTimeManager.h"
#include "PLAYER\CPlayerManager.h"
#include "TREASURE/CTreasureManager.h"
#include "GOAL/CGoalManager.h"
#include "ATTACK\CAttackManager.h"
#include "THREAD\CThreadManager.h"
#include "CGame.h"
#include "../../JUDGE/CJudgeManager.h"
#include "FIELD/CFieldManager.h"
#include "UI\CCountDown.h"
#include "../../BACKGROUND/CBackGroundManager.h"
#include "EFFECT\CEffectManager.h"
#include "UI\CWinDraw.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

// ゲームの制限時間
static const short GAME_TIME = 200;

// 宝物の場所
static const D3DXVECTOR3 TREASURE_POS = D3DXVECTOR3(647, 315, 0);

// ゴール
static const D3DXVECTOR3 GOAL_POS[GOAL_MAX] = {
	D3DXVECTOR3(50, 50, 0),
	D3DXVECTOR3(1230, 50, 0),
	D3DXVECTOR3(50, 670, 0),
	D3DXVECTOR3(1230, 670, 0),
};
static const short GOAL_PLAYER_NUMBER[GOAL_MAX] = {
	0, 1, 2, 3
};

// 背景のスクロールの速さ
static const float BG_SPEED = 2.0f;

// リザルトロゴを表示してからフェードするまでのカウント数
static const short RESULT_LOGO_TO_FADE_INTERVAL = 180;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGame::CGame(void)
{
	m_pPause = NULL;
	m_pTimeManager = NULL;
	m_pPlayerManager = NULL;
	m_pAttackManager = NULL;
	m_pThreadManager = NULL;
	m_pGoalManager = NULL;
	m_pJudgeManager = NULL;
	m_pFieldManager = NULL;
	m_pCountDown = NULL;
	// 一回BGMを鳴らしたか
	m_bPlaySoundOnece = false;

	// 簡易リザルトから次のシーン遷移までのカウンター
	m_nResultCount = 0;

	// プレイヤ操作可能フラグ
	m_bPlayerControl = false;
	// ゲーム終了フラグ
	m_bGameOver = false;
	// 勝ったプレイヤー番号
	m_nWinPlayerNum = -1;
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
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// ポーズ作成
	m_pPause = new CPause(pDevice);
	m_pPause->Init(DEFFAULT_PAUSE_POS, (float)SCREEN_WIDTH, DEFFAULT_PAUSE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ゲームの時間管理と残り時間表示
	m_pTimeManager = new CTimeManager(pDevice, GAME_TIME);
	m_pTimeManager->Init();

	// 攻撃マネージャ生成
	m_pAttackManager = new CAttackManager(pDevice);
	m_pAttackManager->Init();

	// 糸マネージャ生成
	m_pThreadManager = new CThreadManager(pDevice);
	m_pThreadManager->Init();

	// 宝物生成
	m_pTreasureManager = new CTreasureManager(pDevice);
	m_pTreasureManager->Init();
	m_pTreasureManager->CreateTreasure(TREASURE_POS);

	// ゴール生成
	m_pGoalManager = new CGoalManager(pDevice, this);
	m_pGoalManager->Init();
	m_pGoalManager->CreateGoal(
		const_cast<D3DXVECTOR3*>(GOAL_POS),
		const_cast<short*>(GOAL_PLAYER_NUMBER), this);

	// 生成するフィールド番号取得
	m_nStageNum = CManager::GetStageNum();

	// フィールド作成
	m_pFieldManager = new CFieldManager;
	m_pFieldManager->LoadField(m_pD3DDevice, (CFieldManager::FIELD_TYPE)m_nStageNum);

	// 生成するプレイヤの数を取得
	m_nPlayerNumManual = CManager::GetPlayerManualNum();
	m_nPlayerNumCpu = PLAYER_MAX - m_nPlayerNumManual;

	// プレイヤ生成
	m_pPlayerManager = new CPlayerManager(m_pAttackManager, m_pThreadManager, m_pEffectManager);
	m_pPlayerManager->Init(m_nPlayerNumCpu, m_nPlayerNumManual, &m_bPlayerControl);

	// 背景作成
	m_pBackGroundManager = new CBackGroundManager(pDevice);
	m_pBackGroundManager->Init();
	m_pBackGroundManager->CreateBG(TEXTURE_BG_0);
	m_pBackGroundManager->CreateBG(TEXTURE_BG_1, BG_SPEED);

	// ジャッジ作成
	m_pJudgeManager = CJudgeManager::Create(m_pPlayerManager);

	// カウントダウン
	m_pCountDown = new CCountDown(m_pD3DDevice, &m_bPlayerControl);
	m_pCountDown->Init();

	// エフェクトマネージャー
	m_pEffectManager = new CEffectManager(m_pD3DDevice);
	m_pEffectManager->Init();

	// 勝ち引き分けを表示するマネージャ
	m_pWinDrawLogo = new CWinDraw(m_pD3DDevice);
	m_pWinDrawLogo->Init();
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGame::Uninit(void)
{
	if (m_pWinDrawLogo){
		m_pWinDrawLogo->Uninit();
		SAFE_DELETE(m_pWinDrawLogo);
	}
	if (m_pEffectManager){
		m_pEffectManager->Uninit();
		SAFE_DELETE(m_pEffectManager);
	}
	if (m_pCountDown){
		m_pCountDown->Uninit();
		SAFE_DELETE(m_pCountDown);
	}
	if (m_pBackGroundManager){
		m_pBackGroundManager->Uninit();
		SAFE_DELETE(m_pBackGroundManager);
	}
	if (m_pJudgeManager){
		m_pJudgeManager->Uninit();
		SAFE_DELETE(m_pJudgeManager);
	}
	if (m_pFieldManager){
		m_pFieldManager->Uninit();
		SAFE_DELETE(m_pFieldManager);
	}
	if (m_pThreadManager){
		m_pThreadManager->Uninit();
		SAFE_DELETE(m_pThreadManager);
	}
	if (m_pAttackManager){
		m_pAttackManager->Uninit();
		SAFE_DELETE(m_pAttackManager);
	}
	if (m_pPlayerManager){
		m_pPlayerManager->Uninit();
		SAFE_DELETE(m_pPlayerManager);
	}
	if (m_pGoalManager){
		m_pGoalManager->Uninit();
		SAFE_DELETE(m_pGoalManager);
	}
	if (m_pTreasureManager){
		m_pTreasureManager->Uninit();
		SAFE_DELETE(m_pTreasureManager);
	}
	if (m_pTimeManager){
		m_pTimeManager->Uninit();
		SAFE_DELETE(m_pTimeManager);
	}

	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	// カウントダウンの更新
	if (m_pCountDown->Update() && !m_bPlaySoundOnece){
		// カウントダウンが終了したのでBGMを流す
		CManager::PlaySoundA(SOUND_LABEL_BGM000);
		// 一回しか通らない
		m_bPlaySoundOnece = true;
	}

	// 背景の更新
	m_pBackGroundManager->Update();

	if (!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		// プレイヤの操作ができるなら
		if (m_bPlayerControl){
			// マネージャー更新
			m_pPlayerManager->Update();
			m_pTimeManager->Update();
			m_pTreasureManager->Update();
			m_pJudgeManager->Update();
			m_pFieldManager->Update();
		}
		// 残り時間が0になったらDraw
		if (m_pTimeManager->GetRemaining() == 0){
			SetDraw();
			CManager::PlaySoundA(SOUND_LABEL_SE_END);
		}

		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// リザルトへ
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}

		// ゲームクリアフラグ
		if (m_bGameOver)
		{
			Result();
		}
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

//*****************************************************************************
// 勝ったプレイヤ番号のセット 一回だけ呼ばれる
//*****************************************************************************
void CGame::SetWinPlayer(short num){

	// すでにゲームが終わっていたら何もしない
	if (m_bGameOver){
		return;
	}
	m_nWinPlayerNum = num;

	// ゲーム中簡易リザルトの更新フラグをON
	m_bGameOver = true;

	m_pWinDrawLogo->CreateWinLogo();

	CManager::PlaySoundA(SOUND_LABEL_SE_END);
}
//*****************************************************************************
// 引き分けにする 一回だけ呼ばれる
//*****************************************************************************
void CGame::SetDraw(){

	// すでにゲームが終わっていたら何もしない
	if (m_bGameOver){
		return;
	}

	// ゲーム中簡易リザルトの更新フラグをON
	m_bGameOver = true;

	// ロゴの表示
	m_pWinDrawLogo->CreateDrawLogo();
}

//*****************************************************************************
// ゲーム中のリザルトシーンの更新 毎フレーム呼ばれる
//*****************************************************************************
void CGame::Result(){

	// 引き分け
	if (m_nWinPlayerNum == -1){
		// ＤＲＡＷロゴの更新（アニメーション）
	}
	m_nResultCount++;

	if (m_nResultCount > RESULT_LOGO_TO_FADE_INTERVAL){
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		// リザルトへ
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		// 勝ったプレイヤ番号をリザルトに送るためManagerに送る
		m_pManager->SetWinPlayerNum(m_nWinPlayerNum);
	}

	//	if (m_pLogoDraw)
	//		m_pLogoDraw->Update();

}
//----EOF-------